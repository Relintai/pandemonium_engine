# Pandemonium Engine TODOs

## WFC module

- Need to rework the readme. Also link to the sample repo. (https://github.com/Relintai/wfc_module_samples)
- TilingWaveFormCollapse should not generate the images themeslves, rather it should just use internal ids, and return those to you. It could store variants. -> a derived class chould be mamde that adds image generation on top, but in a friendlier way.
- Array2D and 3D's getters and setters that need coordinates use a reversed order compared to everything in the engine. This is super dangerous, and should be changed. (Currently: get(y, x). Should be get(x, y)).
- The classes need smaller fixes and touchups.
- There are probably a few lingering bugs, as some examples give bad results.
- Need to make sure wave size, out size, and size related things are consistent, and intuitive, as right now they are neither.
- The bindings will likely need more improvements.
- Would be nice, if the algorithm could be parametrized from gdscript aswell.
- Maybe addding Array2D, and Array3D like classes to the engine would be a good idea.

## Physics

- Add a multi thread wrapper around the physics server (and nav server).
- Add a method to the physics server that lets the caller thread wait for the physics update, and wait for each thread to finish.
- These 2 changes would enable heavy simplifications to most Props, Voxelman, and Terraman generation code, as they could just directly use the physics server in multi threaded jobs without crashing. The entire update mode thing could be removed.

## Terraman, Voxelman, Props

- MaterialCache class (classes) should be either core or just an another module. (I think the build system has module dependencies implemented.)

## Terraman, Voxelman

- The rid storage API is using dicts currently, and it's pretty awful to mess with. A new mesh storage class should be added instead. Since more than one module needs it either should be core or a new module.  The api could just return a Ref<> to the storage class. And the class could be inherited from when needed. Should probably contain some generic api which allows you to store most things without really messing around too much. Like an api for mesh rids, navmesh rids, and collider rids, and generic rids.

## Editor

- Add support for enabling / disabling editor plugins, and also add a gui for this. (Like how the gdscript addons can be enabled/disabled, but for built in c++ stuff.) It will need support for either exlusion, or dependencies. (Dependencies would be better.)
- Modularize the editor. Should probably add a new editor_modules folder (Just normal modules, but they won't get compiled if tools are disabled).

## Engine

- Organize the core folder a bit better.
