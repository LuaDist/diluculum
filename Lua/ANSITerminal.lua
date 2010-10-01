--
-- ANSITerminal.lua
-- Text coloring, cursor positioning and similar tricks, based on ANSI escape
-- codes.
-- Leandro Motta Barros
--

local P = { }
ANSITerminal = P


-- Import names and use a non-global environment
local assert = assert
local file = io.file
local open = io.open
local output = io.output
local pairs = pairs
local setmetatable = setmetatable
local write = io.write
setfenv (1, P)


-- ANSI codes representing special commands to the terminal. They are public, so
-- that anyone can use them directly. Just notice that the 'Terminal' class (see
-- below) manages it's own state, and mixing calls to its methods with direct
-- use of these codes will mess things up.
ANSICodes = {
   eraseDisplay = "\27[2J",

   reset = "\27[0m",
   bold = "\27[1m",
   underline = "\27[4m",
   blink = "\27[5m",
   reverse = "\27[7m",

   fgBlack = "\27[30m",
   fgRed = "\27[31m",
   fgGreen = "\27[32m",
   fgYellow = "\27[33m",
   fgBlue = "\27[34m",
   fgMagenta = "\27[35m",
   fgCyan = "\27[36m",
   fgWhite = "\27[37m",

   bgBlack = "\27[40m",
   bgRed = "\27[41m",
   bgGreen = "\27[42m",
   bgYellow = "\27[43m",
   bgBlue = "\27[44m",
   bgMagenta = "\27[45m",
   bgCyan = "\27[46m",
   bgWhite = "\27[47m",
}

-- Helper table, mapping color names to color codes (for foreground color).
local FGColorTable = {
   black = ANSICodes.fgBlack,
   red = ANSICodes.fgRed,
   green = ANSICodes.fgGreen,
   yellow = ANSICodes.fgYellow,
   blue = ANSICodes.fgBlue,
   magenta = ANSICodes.fgMagenta,
   cyan = ANSICodes.fgCyan,
   white = ANSICodes.fgWhite
}

-- Helper table, mapping color names to color codes (for background color).
local BGColorTable = {
   black = ANSICodes.bgBlack,
   red = ANSICodes.bgRed,
   green = ANSICodes.bgGreen,
   yellow = ANSICodes.bgYellow,
   blue = ANSICodes.bgBlue,
   magenta = ANSICodes.bgMagenta,
   cyan = ANSICodes.bgCyan,
   white = ANSICodes.bgWhite
}


-- A 'Terminal' is an abstraction of device capable to interpret ANSI codes. It
-- is usually the standard output, but it can also be, for example, a file (so
-- that when 'cat'ing the file, it will be displayed in color.
Terminal = {
   fgColor = "white",
   bgColor = "black",
   bold = false,
   blink = false,
   underline = false,
   reverse = false
}


-- Constructs a new 'Terminal'. The parameter 'file' is the file descriptor to
-- where ANSI codes will be send.
function Terminal:new(file)
   assert (file ~= nil, "Terminal:new(): Passed 'nil' as 'file' parameter!")

   o = { file = file }
   setmetatable (o, self)
   self.__index = self
   return o
end


-- Writes 'what' to the file associated with this 'Terminal'. Actually, this
-- just calls 'file:write(what)'.
function Terminal:write(what)
   self.file:write(what)
end


-- Outputs the ANSI codes necessary to make the internal state "active".
function Terminal:applyState()
   self.file:write (ANSICodes.reset..FGColorTable[self.fgColor]
                    ..BGColorTable[self.bgColor])

   for k,v in pairs{"bold", "blink", "underline", "reverse"} do
      if self[v] then self.file:write (ANSICodes[v]) end
   end
end


-- Reset the terminal to its default settings.
function Terminal:reset()
   for k,v in pairs (Terminal) do
      self[k] = v
   end

   self:applyState()
end


-- Sets the foreground color to 'color'. 'color' must be one of the folowing
-- strings: "black", "red", "green", "yellow", "blue", "magenta", "cyan",
-- "white".
function Terminal:setFGColor(color)
   assert (FGColorTable[color],
           "Terminal:setFGColor(): '"..color.."' is not a valid color!")

   self.fgColor = color
   self.file:write(FGColorTable[self.fgColor])
end


-- Sets the background color to 'color'. 'color' must be one of the folowing
-- strings: "black", "red", "green", "yellow", "blue", "magenta", "cyan",
-- "white".
function Terminal:setBGColor(color)
   assert (BGColorTable[color],
           "Terminal:setBGColor(): '"..color.."' is not a valid color!")

   self.bgColor = color
   self.file:write(BGColorTable[self.bgColor])
end


-- Sets the blink flag to 'blink' (either 'true' or 'false')
function Terminal:setBlink(blink)
   if self.blink ~= blink then
      self.blink = blink
      self:applyState()
   end
end


-- Sets the bold flag to 'bold' (either 'true' or 'false')
function Terminal:setBold(bold)
   if self.bold ~= bold then
      self.bold = bold
      self:applyState()
   end
end


-- Sets the underline flag to 'underline' (either 'true' or 'false')
function Terminal:setUnderline(underline)
   if self.underline ~= underline then
      self.underline = underline
      self:applyState()
   end
end


-- Sets the reverse flag to 'reverse' (either 'true' or 'false')
function Terminal:setReverse(reverse)
   if self.reverse ~= reverse then
      self.reverse = reverse
      self:applyState()
   end
end


-- Clears the screen.
function Terminal:clear()
   self.file:write(ANSICodes.eraseDisplay)
end


-- Moves the cursor to coordinates ('x', 'y'), measured from the screen's
-- top-left corner.
function Terminal:gotoXY(x,y)
   self.file:write("\27["..y..";"..x.."H")
end


-- Moves the cursor 'lines' lines up (default is 1).
function Terminal:cursorUp(lines)
   lines = lines or 1
   self.file:write("\27["..lines.."A")
end


-- Moves the cursor 'lines' lines down (default is 1).
function Terminal:cursorDown(lines)
   lines = lines or 1
   self.file:write("\27["..lines.."B")
end


-- Moves the cursor 'columns' columns to the right (default is 1).
function Terminal:cursorRight(columns)
   columns = columns or 1
   self.file:write("\27["..columns.."C")
end


-- Moves the cursor 'columns' columns to the left (default is 1).
function Terminal:cursorLeft(columns)
   columns = columns or 1
   self.file:write("\27["..columns.."D")
end
