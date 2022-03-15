FastNoise for Godot Engine
=============================

Description
-------------

This is an integration of the [FastNoise C++ library](https://github.com/Auburns/FastNoise) for Godot Engine.
It uses Simplex Noise for some of its generators, which is [patented](https://www.google.com/patents/US6867776) for image generation.
If you want to avoid the patent, you can try OpenSimplex, for which I also made a [module](https://github.com/Zylann/godot_opensimplex).

I still prefer FastNoise because it has more features, doesn't allocates dynamic memory and works with a single class.

Install
--------

You have to get the source code of Godot to compile it with the module. Copy the contents of the repository inside a `fastnoise`  directory under Godot's `modules` folder. The name is important for the module to compile properly.

Example git command: `git clone https://github.com/Zylann/godot_fastnoise.git fastnoise`
