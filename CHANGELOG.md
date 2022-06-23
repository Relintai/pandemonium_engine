# Changelog

All notable changes to this project will be documented in this file.

## [Master] 

- Nothing since the last changeset.

## [3.6.0]

### Added

#### Core

- Added contains(), find(), and clear() to PoolVector. Also added bindings for them.
- Added a bound core logger class.

#### Modules

- Ported the mesh data editor addon to c++, and now it's a part of the mesh_data_resource module.
- Ported my fork of Godoxel to c++, and now it's part of the engine. (paint module)
- Added a new wfc (WaweFormCollapse) module. I used https://github.com/math-fehr/fast-wfc 's code as a base for it. Samples repository: https://github.com/Relintai/wfc_module_samples
- Ported mat_maker_gd to c++, and now it's part of the engine. (material_maker module)

#### Module - Props

- Added Z-offset support for TiledWalls. It can be used to offset wall tiles to create 3d wall features. It's missing collider support at the moment.

#### Other

- Added TODO.md.
- Added some of my game project helper scripts, and made them work from their new place.

#### UI

- Ported the EditorZoomWidget from Godot4.

#### Platform

- Added frt1 (https://github.com/efornara/frt/tree/1.x).
- Added efornara's frt2 platform. (https://github.com/efornara/frt - 2.0 branch). It won't build when selected right now, due to core changes.

### Removed

#### Modules

- webp.
- webm.
- remnants of libvpx, and libwebp, and xatlas.

### Fixed

#### Platform

- Fixed low processor mode on android. The fix was inspired by https://github.com/godotengine/godot/pull/59606, although I did it differently.

#### Engine

- Fix compile when using llvm on linux.

#### Modules

- Set Prop2D and Prop module's convert button style to flat, so it's in the expected style.

## [3.5.0]

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
- Ported NetworkedController & SceneRewinder (https://github.com/godotengine/godot/pull/37200).

#### Engine

- Added Vector2i, Vector3i, Rect2i, PoolVector2i, PoolVector3i, and StringName as core classes.
- Added an is_null() helper method to Variant.
- Backported the 3.x android editor port pr by m4gr3d and thebestnom. (https://github.com/godotengine/godot/pull/57747)
- Added most of the build tools from the broken seals repo, and cleaned them up a bit.

#### Other

- Added the build containers from BrokenSeals.

### Changed

#### Engine

- Added options for disabling RTTI and exceptions for every platform, and by default I disabled them.
- Reworked the help menu.
- Replaced the godot icon.
- Mass replaced the word godot to pandemonium.

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
- bullet

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
- Removed the downloader from the export templates manager.
- Removed the translations, except for hungarian, as I won't be able to keep them updated.
- Removed the online docs link from the top of the script editor.

#### module: ui_extensions

- Removed TMeshInstance, and changed the core MeshInstance to wrok the way I need it instead.

#### other

- Removed the github issue tamplates.

### Fixed

#### Modules

- Added material cache support for Voxelman, and Terraman's bake texture mesher step type.

#### Editor Docs

- Fixed all warnings.

#### Editor Docs

- Limited the length of the default parameter in the class docs. This fixed the properties display for classes like Entity.

#### Other

- Github actions have been simplifed and fixed.
- Godot's regression test tool have been updated / fixed.
- Fixed all crashed the regression test tool found by throwing invalid / unexpected arguments at methods.
- Cleanups to the docs.
- Fix errors / update the osx scripts.
- Annotated the podman build all script better, also changed the log file names, and added more variants.
- improvements and fixes to the build container.

### Backports

- https://github.com/godotengine/godot/commit/6c2bd1203126586c6cded4c2bcf210926714ab42
- https://github.com/godotengine/godot/commit/c2c0c8067d3c1b5bc9ca72bdb805690227a28575
- https://github.com/godotengine/godot/commit/62fa80afd7734c40aef4148d71a43cd6c52e26ab
- https://github.com/godotengine/godot/commit/fd968157a4b9145b245e3d749e1ee7e12ae9ae8b
- https://github.com/godotengine/godot/commit/c0bd05762e0398589f78f2f0c4a5f4efdce786f0
- https://github.com/godotengine/godot/commit/72837b8ecd52e1734360a83ebb129ee477cc413a
- https://github.com/godotengine/godot/commit/88c723c33c219113ec0e533942363ceccf422229
- https://github.com/godotengine/godot/commit/d76c9f8d92fe3533244fdf74679c67475bec2983
- https://github.com/godotengine/godot/commit/658877c350dc3062d633902aac06050e1c3352e0
- https://github.com/godotengine/godot/commit/d537a88299ee1faa79ea81d6acd0774e59c3a288
- https://github.com/godotengine/godot/commit/b33eed23aef39db58dac563dc14d7110f16eca50
- https://github.com/godotengine/godot/commit/797321fec45df71f1f7d4fcca4e9af30d11853b9
- https://github.com/godotengine/godot/commit/c77b71003532237aeef466ce46c12ff01948bc2e
- https://github.com/godotengine/godot/commit/bb257543d873ed05931fc8d46f7b5321598f32ee
- https://github.com/godotengine/godot/commit/cf1b3fdd55989fe7b2d8ef665ce5fefec4db4cb7
- https://github.com/godotengine/godot/commit/480543f681b50c3037f161bd0ecd531e78b87912
- https://github.com/godotengine/godot/commit/09e4672f0d1e56b7cbd70c479192a1d5a5c1a6de
- https://github.com/godotengine/godot/commit/e822145be2f7a95c5bac9986a534dccde482afbe
- https://github.com/godotengine/godot/commit/3fe37f963595cb1887110f9070275d72c54ff0a6
- https://github.com/godotengine/godot/commit/937c1a716c87e48ba0d1c097747e0438dc076f67
