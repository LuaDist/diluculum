--
-- CommandLineParser.lua
-- A command-line parser for Lua, somewhat inspired by the one used
-- by GoboLinux's scripts.
-- Leandro Motta Barros
--

local P = { }
CommandLineParser = P


-- Import names and use a non-global environment
-- local assert = assert
local assert = assert
local io = io
local ipairs = ipairs
local pairs = pairs
local table = table
local string = string
local tonumber = tonumber
local tostring = tostring
local type = type
setfenv (1, P)

local OptionsTable = nil
local Usage = ""
local ShortDescription = ""
local OptionsByOption = { }
local UserSelectedOptions = { } -- Options selected by the user


-- Initializes the command-line parser.
-- Parameter 'options' must be a list of tables, with each table representing
-- one available option. See test code for examples.
-- Parameter 'usage' must be a string, describing how to call the program.
function Init (options, shortDescription, usage)
   -- Ensure that 'options' is in a proper format
   assert (type(options) == "table", "'options' parameter must be a table.")

   for k,v in ipairs (options) do
      assert (type(v) == "table", "All entries in 'options' must be tables.")

      assert (type(v[1]) == "string",
              "First element of an 'options' entry must be a string.")

      assert (v[2] == "flag" or v[2] == "boolean" or v[2] == "string"
              or v[2] == "number",
              "Second element of an 'options' entry must be 'flag', "
              .."'boolean', 'string' or 'number' (got '"..v[2].."').")

      assert (type(v[3]) == "table",
              "Third element of an 'options' entry must be a table.")

      for k2,v2 in ipairs (v[3]) do
         assert (type(v2) == "string",
                 "Options (in 'options' third element) must be strings.")
         assert (string.sub(v2, 1, 1) == "-",
                 "Options (in 'options' third element) must begin with '-'"
                 .." (got '"..v2.."').")
      end

      assert (type(v[4]) == "string",
              "Fourth element of an 'options' entry must be a string.")

      if v[2] ~= "flag" then
         assert (type(v[5]) == v[2], "Default values (in 'options' fifth "
                 .."element) must match the option type (and '"..tostring(v[5])
                 .."' is not a '"..v[2].."').")
      end
   end

   -- Initialize global variables
   OptionsTable = options
   Usage = usage
   ShortDescription = shortDescription

   for k, optionDescription in pairs (OptionsTable) do
      -- Populate an auxiliary table with options indexed by the option string
      -- as the user is expected to type in the command-line.
      for k2, option in pairs (optionDescription[3])do
         OptionsByOption[option] = optionDescription
      end

      -- Populate the table of user-selected options with defaults
      if optionDescription[2] == "flag" then
         UserSelectedOptions[optionDescription[1]] = false
      else
         UserSelectedOptions[optionDescription[1]] = optionDescription[5]
      end
   end
end


-- Parses the arguments 'args', filling internal data structures. Processed
-- entries are removed from 'args'. Returns 'true' on success. On failure,
-- writes a message to standard output and returns 'false'. 'false' will also be
-- returned if the user passed '-h' or '--help', in which case, an option
-- summary will also be printed to the standard output.
function Parse(args)

   assert (OptionsTable ~= nil, "Parse: Command-line parser was not properly "
           .."initialized. Was 'CommandLineParser.Init()' called?")

   -- Prints an usage summary to the standard error.
   local function printHelp()
      io.stderr:write ("Usage: "..args[0].." "..Usage.."\n"..ShortDescription
                       .."\n\nAvailable command-line options:\n")

      for k,v in ipairs (OptionsTable) do
         if v[2] == "boolean" then
            io.stderr:write ("    "..table.concat(v[3], " <boolean>, ")
                             .." <boolean>\n")
         elseif v[2] == "string" then
            io.stderr:write ("    "..table.concat(v[3], " <string>, ")
                             .." <string>\n")
         elseif v[2] == "number" then
            io.stderr:write ("    "..table.concat(v[3], " <number>, ")
                             .." <number> \n")
         else
            io.stderr:write ("    "..table.concat(v[3], ", ").."\n")
         end

         io.stderr:write ("        "..v[4].."\n")
      end

      io.stderr:write ("    -h, --help\n        Prints this help screen.\n")

   end

   -- Parses a boolean option parameter. 'cmdLine' is the command-line option
   -- used (like "--color"). 'option' is the option name. And 'param' is the
   -- parameter gotten by the option (e.g., "true").
   -- On success, returns 'true'. On failure, prints an error message to
   -- standard output and returns 'false'.
   local function parseBoolean (cmdLine, option, param)
      if param == "true" or param == "yes" then
         UserSelectedOptions[option] = true
      elseif param == "false" or param == "no" then
         UserSelectedOptions[option] = false
      else
         io.stderr:write (args[0]..": '"..cmdLine.."' should receive boolean "
                          .."parameter ('true', 'false', 'yes' and 'no' "
                          .."are accepted).\n")
         return false
      end

      return true
   end


   -- Parses a number option parameter. 'cmdLine' is the command-line option
   -- used (like "--count"). 'option' is the option name. And 'param' is the
   -- parameter gotten by the option (e.g., "123").
   -- On success, returns 'true'. On failure, prints an error message to
   -- standard output and returns 'false'.
   local function parseNumber (cmdLine, option, param)
      local num = tonumber(param)
      if type(num) == "number" then
         UserSelectedOptions[option] = num
      else
         io.stderr:write (args[0]..": '"..cmdLine.."' should receive a numeric "
                          .."parameter.\n")
         return false
      end

      return true
   end


   local argsToRemove = { } -- indices of parsed parameters, to be removed
                            -- from 'args'.

   -- Check every parameter passed
   for i = 1, table.getn(args) do
      local arg = args[i]

      -- If we got "--", stop parsing now
      if arg == "--" then
         table.insert (argsToRemove, 1, i)
         break
      end

      -- Check for help flags
      if arg == "-h" or arg == "--help" then
         printHelp()
         return false
      end

      -- If we got something starting with "-", treat as a command-line option
      if string.sub (arg, 1, 1) == "-" then
         if not OptionsByOption[arg] then
            io.stderr:write(args[0]..": '"..arg.."' is not a recognized option. "
                            .."Use '-h' or '--help' for help.\n")
            return false
         end

         local option = OptionsByOption[arg]
         table.insert (argsToRemove, 1, i)

         if option[2] == "flag" then
            UserSelectedOptions[option[1]] = true
         else
            i = i + 1
            table.insert (argsToRemove, 1, i)

            if option[2] == "boolean" then
               if not parseBoolean (arg, option[1], args[i]) then
                  return false
               end
            elseif option[2] == "string" then
               UserSelectedOptions[option[1]] = args[i]
            elseif option[2] == "number" then
               if not parseNumber (arg, option[1], args[i]) then
                  return false
               end
            end
         end
      end
   end

   -- Remove from 'args' the parsed parameters
   for k,v in pairs (argsToRemove) do
      table.remove(args, v)
   end

   return true
end


-- Returns the value of a given option ('nil' if the requested option does not
-- exist).
function GetOption(option)
   return UserSelectedOptions[option]
end
