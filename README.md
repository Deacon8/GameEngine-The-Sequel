# The Sequel

This has much fewer features than the previous, and was more of an attempt to iron out issues I had with the first one.

I wrote this one with C++, rather than C, and in general was a lot more generous with using external libraries.

I ended up using SDL rather than GLFW, and used a glTF loader I found online, rather than use the old OBJ loader I wrote for the first one.

If you've read the readme for the original game engine I made, you'll know that I had a lot of trouble with transformations. I tried to iron out those problems, but I still hadn't taken the matrix math course so it's still a bit messed up, albeit more functional.

The main problem I ran into for this one was creating an entity component system. I wanted to avoid Object Oriented methods, even though I was using C++, as I used them extensively in my C# days. I attempted to create generic component arrays with void pointers, which the compiler was not happy about, but overall, I learned much more.
