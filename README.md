# Pandemonium Engine

<p align="center">
  <a href="https://github.com/Relintai/pandemonium_engine">
    <img src="logo_outlined.svg" width="400" alt="Pandemonium Engine logo">
  </a>
</p>

A 3.x [Godot Engine](https://godotengine.org) fork where I hack and slash and cause mayhem and destruction to all things good and godot.

Similar idea going from godot 3.x to godot 4.x, but taken in a completely different direction.

Currently this engine is a weird amalgamation of godot 3.x, 4.x, and lots of custom features.

Most of the design decisions went into making everything simple for people that knows (or wants to know) what is going on. 

It contains all of my currently in use engine modules.

See the [changelog](https://github.com/Relintai/pandemonium_engine/blob/master/CHANGELOG.md) for a more comprehensive list of changes.

## 2D and 3D cross-platform game engine

Pandemonium Engine is a feature-packed, cross-platform game engine to create 2D and 3D games from a unified interface.
It provides a comprehensive set of common tools, so that users can focus on making games
without having to reinvent the wheel. Games can be exported with one click to a
number of platforms, including the major desktop platforms (Linux, macOS,
Windows), mobile platforms (Android, iOS), as well as Web-based platforms
(HTML5) and [consoles](https://github.com/Relintai/pandemonium_engine_docs/blob/master/03_usage/13_platform/01_consoles.md).

## Free, open source

Pandemonium is completely free and open source under the very permissive 
[MIT license](https://github.com/Relintai/pandemonium_engine/blob/master/LICENSE.txt).
No strings attached, no royalties, nothing. The users' games are theirs, down
to the last line of engine code.

Pandemonium's development is fully independent.

### Godot

Before being open sourced in [February 2014](https://github.com/godotengine/godot/commit/0b806ee0fc9097fa7bda7ac0109191c9c5e0a1ac),
Godot had been developed by [Juan Linietsky](https://github.com/reduz) and
[Ariel Manzur](https://github.com/punto-) (both still maintaining the project) for several
years as an in-house engine, used to publish several work-for-hire titles.

### Pandemonium

The Godot engine developers decided to remove the GLES2 backend in Godot 4.x so they can pursue the more modern graphics API-s.

After quite a while of thinking and experimentation however I decided that I still need (and want) the GLES2 renderer for my games,
I also needed some of the new features from godot 4.x.

So in early 2022 I decided that I'll fork godot 3.x, and backport anything that I
need for my games / projects. While here I also used the opportunity to modularize the engine more, 
and fix issues I had with it. I also added my engine modules to it by default, and
ended up writing lots of custom things.

Keep in mind, that I'm not really an engine dev. Which probably sounds completely bonkers from someone who develops and maintains his own
godot fork almost non-stop. What I mean is that this engine is being developed to be used by my other projects. This brings some downsides,
as this project cannot openly accept anything that anyone wants (like other engines). Fortunately this has never been an issue,
as I don't really try to get people to use this engine, but if this ever becomes a problem I'll create an official "community edition"
fork, that is much more open to outside influences.

This being said, the engine actually tries to be modular as much as possible, while being also easily extendible, and as simple to understand
and use as possible with a wide selection of platforms. All this without constraints that make building apps with the
required complexities of today harder than they need to be.

## Getting the engine

### Binary downloads

You can download binaries from the github actions tab [[here]](https://github.com/Relintai/pandemonium_engine/actions), 
or the releases tab [[here]](https://github.com/Relintai/pandemonium_engine/releases).

### Compiling from source

[See the official docs](https://github.com/Relintai/pandemonium_engine_docs/tree/master/05_engine_development/01_compiling)
for compilation instructions for every supported platform.

## Documentation and demos

The documentation is available [[Here]](https://github.com/Relintai/pandemonium_engine_docs).
It includes all official pandemonium demos.

The class docs are accessible from the editor.

You can also look at the official 3.x Godot documentation, it will work mostly (sometimes with trivial modifications). 

It's also worth looking at official godot 3.x resources, like this [awesome Godot list](https://github.com/godotengine/awesome-godot),
and there are also a number of other [godot learning resources](https://docs.godotengine.org/en/latest/community/tutorials.html)
provided by the community, such as text and video tutorials, demos, etc.

## Some Long term plans

These are the planned new features. On top of this there will be lots of unplanned things,
also improvements to the engine as a whole.

### Migrate the codebase back to c++03

Currently the codebase is mostly c++03 with some c++11, and c++14 (std threading) sprinkled in.

I'm not sure whether it's just me getting old, or maybe it's just timing (I started with 
Libgdx + java 7, then went to Unity + dotnet mono 2.0, then Godot 3.1). I like this sipmlicity. It makes
the codebase a lot easier to work with. It also simplifies the tooling required, which
could come in handy in the future.

After lots of thinking, it's probably the best to just say the target is c++03 instead of writing some
specification of used features.

There are 3 bigger issues (none of them are too bad though):

- Threading was modernized to use the std namespace, but it also introduced a nice way of overriding the default
  for a platform.
- Atomics were completely reworked. The old atomic calls need to be merged into the new api.
- Templates are a bit more strict: `Vector<Vector<int>>` needs a space between the last `>>`-s: `Vector<Vector<int> >`.
  This can be done with reverting the clang format rules, and running it on the project. It will jutst create a big diff.

Other than this it will only need small fixes.

However some third party modules might need higher c++ standard. At first they will just get it. Then we'll see.

Note that this would only be an extremely minor break, and only for engine modules. Also they would only need to specify
a different c++ version in their SCSub file (for which there are plenty of examples).

### CScript

Implement and enable CScript. CScript will be a scripting language similar to gdscript, but
it will use a C-style syntax.

It would be extremely cool if it could just interpret engine module c++ directly, but that
might be a bit too much. Maybe that could be an another module. For this to work, solving .cpp and header 
file separation are the biggest problem in my opinion due to how the engine works internally. Other than this
it's mostly just parsing (not trivial, but relatively simple in the grand scheme of things).
(Note that having the codebase use c++03 also helps with this!)

### MarkdownLabel

A RichTextLabel, but for markdown would be nice to have.

### In-editor docs, and in-editor docs hosting.

Since now the docs are in markdown, if the MarkDown label is implemented the docs could also be read directly using the editor itself.

Obviously it will need to be pointed to a folder, as the docs are quite big, but it would be realtively trivial to implement.

Also it could host to a web browser using the HTTPServer. (The docs repo has a demo that does this already. It's like 200 lines of code.)

### UWP

Figure out how to build a working container to the UWP backend, fix it, and add it to the release.

### Shared object modules

All engine modules by default could support being built as a shared object (= dll). This can save with iteration
time when public apis don't change.

Keep in mind, that this is already implemented for some modules, but would be nice to have as a core feature.

### GDNative, and GDNative C++ 

The GDNative c++ binding rework should be finished. It could also make module development a lot simpler
if it's api is extremely similar to the engine's.

It's project setup should also be simplified.

Also GDNative should probably have an api that can register classes without creating resources in the project.

### Website

Setting up a separate vps for the website would be nice, instead of the current domain redirect.
It should also mirror the engine specific repositories and the release zip-s.

It should also look at least acceptable.


