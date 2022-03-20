# Changelog

All notable changes to this project will be documented in this file.

## [Master] 

Currently contains changes up to 825755c5d4f7c33945ccfb0368b48e7fc0d2e495

### Added

#### Modules

- [[Entity Spell System]](https://github.com/Relintai/entity_spell_system) - An entity and spell system c++ godot engine module, for complex (optionally multiplayer) RPGs.
- [[Terraman]](https://github.com/Relintai/terraman) - Terrain and building engine for godot based on voxelman.
- [[Voxelman]](https://github.com/Relintai/voxelman) - A c++ voxel engine module for godot. 
- [[Props]](https://github.com/Relintai/props) - A godot engine module to help with creating/optimizing in-game props.
- [[Terraman2D]](https://github.com/Relintai/terraman_2d) - 2D Terrain and building engine for godot based on terraman.
- [[Props2D]](https://github.com/Relintai/props_2d) - A godot engine module to help with creating/optimizing in-game 2D props.
- [[Thread Pool]](https://github.com/Relintai/thread_pool) - A c++ Godot engine module which makes it easy to run methods in threads.
- [[Texture Packer]](https://github.com/Relintai/texture_packer) - A c++ Godot engine module, to layer, pack, and merge textures at runtime.
- [[Mesh Data Resource]](https://github.com/Relintai/mesh_data_resource) - A Godot c++ engine module. It adds a data-only mesh type with an importer.
- [[Mesh Utils]](https://github.com/Relintai/mesh_utils) - A c++ Godot engine module containing utilites for working with mehses (Like simplification).
- [[Godot Fastnoise]](https://github.com/Relintai/godot_fastnoise) - Zylann's godot_fastnoise, but with compile fixes for 4.0, and a Resource class.
- [[UI Extensions]](https://github.com/Relintai/ui_extensions) - Small extensions engine module for GODOT.
- [[Skeleton Editor]](https://github.com/Relintai/skeleton_editor) - Godot engine module version of TokageItLab's skeleton editor pr, that got deprecated, but [[my godot fork]](https://github.com/Relintai/godot/tree/3.x) has it fully implemented.
- [[rtile_map]](https://github.com/Relintai/rtile_map) - Godot's tilemap, with a few additional features.

### Changed

#### Engine

- Added options for disabling RTTI and exceptions for every platform, and by default I disabled them.

#### Modules

- Removed all godot version support code from my modules. Along with some macros.

#### Branding

- Rebranded the engine. Also managed to create an icon for it.
- New default color scheme.

#### Internals

- On the c++ side I moved lots of includes from the headers to the cpp files to hopefully improve compile and recompile times.
- Moved the include guards to the very top of files. This can potentially speed up builds as the compiler should be able to reject headers simpler.

### Removed

#### Modules

- gridmap
- mono
- denoise
- lightmapper_cpu
- lightmap_raycast
- gdnative
- webxr
- visual_script
- csg
- fbx
- xatlas_unwrap
- arvr
- camera
- movile_vr
- jsonrpc
- webrtc

#### Core classes

- Lightmapper
- BakedLightmap
- Particles, Particles2D. (Kept CPUParticles, CPUParticles2D!)

#### Rendering

- GLES3, along with the VisualServer methods that are not supported in GLES2.
- Removed GIProbes.
- Removed lightmap captures.

#### Platforms

- UWP (I was never once able to succesfully set up the environment for it to compile during the last 3 years).

#### Editor

- Removed the collada importer.
- Removed the obj importer.
- Removed the default skjeleton editor.
- Removed the AssetLibrary.
- Removed VisualShaders.
- Removed the editor feature profiles.
- Removed the VCS plugin.
- Removed TileMap and TileSet. They are still available through the rtile_map module though.
- Removed the ARVR singleton.
- Removed the Camera singleton.
- Removed the editor asset installer.
- Removed the gdscript language server.

### Fixed

#### Editor Docs

- Limited the length of the default parameter in the class docs. This fixed the properties display for classes like Entity.

#### Other

- Github actions have been simplifed and fixed.
- Godot's regression test tool have been updated / fixed.
- Fixed all crashed the regression test tool found by throwing invalid / unexpected arguments at methods.

### Backports

- https://github.com/godotengine/godot/commit/6c2bd1203126586c6cded4c2bcf210926714ab42
- https://github.com/godotengine/godot/commit/c2c0c8067d3c1b5bc9ca72bdb805690227a28575
- https://github.com/godotengine/godot/commit/62fa80afd7734c40aef4148d71a43cd6c52e26ab
- https://github.com/godotengine/godot/commit/fd968157a4b9145b245e3d749e1ee7e12ae9ae8b
- https://github.com/godotengine/godot/commit/c0bd05762e0398589f78f2f0c4a5f4efdce786f0
- https://github.com/godotengine/godot/commit/72837b8ecd52e1734360a83ebb129ee477cc413a

