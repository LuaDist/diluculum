--
-- Test_ANSITerminal.lua
-- Tests for 'ANSITerminal.lua'
-- Leandro Motta Barros
--

require "ANSITerminal"

-- Create a 'Terminal' named 'term' linked to the standard output.
term = ANSITerminal.Terminal:new(io.output())

term:reset()
term:clear()

-- Test all bold/foreground/background colors combinations
term:gotoXY(1,1)

colors = {"black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"}
for k,v in ipairs(colors) do
   term:setBGColor(v)
   for k2,v2 in ipairs(colors) do
      term:setFGColor (v2)
      term:write ("ABC")
   end

   term:setBGColor ("black")
   term:write (" ")
   term:setBGColor(v)
   term:setBold (true)

   for k2,v2 in ipairs(colors) do
      term:setFGColor (v2)
      term:write ("ABC")
   end

   term:setBold (false)
   term:setBGColor ("black")
   term:write ("\n")
end

term:reset()

term:gotoXY (55, 2)
term:write ("Normal")

term:setBlink (true)
term:gotoXY (70, 4)
term:write ("Blink")

term:setBlink (false)
term:setUnderline (true)
term:cursorLeft (22)
term:write ("Underline")

term:setUnderline (false)
term:setReverse (true)
term:cursorDown (3)
term:write ("Reverse")

term:cursorUp()
term:setUnderline (true)
term:setBlink(true)
term:write ("RUB")

term:setReverse (false)

-- Position the cursor in a low enough place (everything below the cursor is
-- erased after program termination -- at least on 'xterm')
term:gotoXY (1, 10)
term:reset()
