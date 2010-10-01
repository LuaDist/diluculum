--
-- Test_CommandLineParser.lua
-- Tests for 'CommandLineParser.lua'
-- Leandro Motta Barros
--

require "CommandLineParser"


TheOptions = {
   { "AutoMode", "flag", { "-a", "--auto" }, "Runs in automatic mode." },
   { "UseColor", "boolean", { "-c", "--color", "--colour" }, "Uses colorized output.", true },
   { "UserName", "string", { "-n", "--name" }, "User name.", "Anonymous" },
   { "LuckyNumber", "number", { "-l", "--lucky" }, "User's lucky number.", 13 },
}

CommandLineParser.Init (TheOptions, "A simple command-line parser test.",
                        "<options> <files> [...]")

if CommandLineParser.Parse(arg) then
   print ("Option for 'AutoMode': "..tostring(CommandLineParser.GetOption("AutoMode")))
   print ("Option for 'UseColor': "..tostring(CommandLineParser.GetOption("UseColor")))
   print ("Option for 'UserName': "..tostring(CommandLineParser.GetOption("UserName")))
   print ("Option for 'LuckyNumber': "..tostring(CommandLineParser.GetOption("LuckyNumber")))
   print ("Option for invalid entry: "..tostring(CommandLineParser.GetOption("Foo")))

   print ("\nRemaining arguments:")

   for k,v in ipairs (arg) do
      print (v)
   end
end
