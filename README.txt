Diluculum started as a collection of "pure Lua" packages, but things
started to get really interesting when I added some C++ to it.

I used to use a class named 'LuaState', that was a pretty low-level
encapsulation of a Lua state. As the time passed, I learned the ways
in which I used Lua most and suddenly realized that life could be
better. So I started to design a new 'LuaState' class. A higher level
'LuaState', that would allow me to do with ease all the Lua things I
did most of the time.

For the lack of a better place to put the new 'LuaState' (and its
related classes) I added it to Diluculum.

And here it is.

Enjoy,

Leandro Motta Barros
