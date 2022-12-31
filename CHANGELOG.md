# Changelog

All notable changes to this project will be documented in this file.

## [Master] 

Nothing yet.

## [3.10.0] 

### Added

#### Core

- Added support for specifying a COPYRIGHT.txt file for modules.

- Never print an error message in Object::get_meta(). 

- Remove now superfluos check from String's operator[]. 
- Add _FORCE_INLINE_ to String's resize(). 
- Added set_length helper method to String.

#### Platforms

##### Android

- Now the desired android arches can be easily specified for the android build when using to the app build script.

#### Modules

##### User

- Added signals for user actions into the user web nodes.
- Added a simple page rendering customization api for UserWebPages.

##### Database 

- Added initialized signal and helper method to the DataBaseManager. It can be used to let automally allocated classes (singletons, main scene classes) in heavily database driven applications know when it's safe to use the db.
- Added docs for the QueryBuilder. 
- Added docs for the TableBuilder. 

##### Web

- Added HTTPSessionManagerDB using the disabled code in HTTPSessionManager, and cleaned up it's (and HTTPSessionManager's) logic.
- Added new tag() and ctag() universal tag helpers to the HTMLBuilder binder class.
- Also added tag() and ctag() helpers to the c++ side HTMLBuilder, and smaller cleanups.
- Added get request parameter support for WebServerRequest. 

#### Backports

- Backported everything up to and including https://github.com/godotengine/godot/commit/0f10eafb38bd54e59fa7fd079ceafd2c07e8be2a 

### Fixed

- Fixed lots typos with codespell. 

#### Editor

- Removed unused variables from the SceneTreeDock.
- Now the Access as Unique Name option is in the right place when right clicking the scene tree.
- Fix regression, and an another issue with touch gui input handling. 

#### Platforms

##### Android

- Don't enable remote debugging in the android editor for now. This fixes black screen when a project encounters a script error.

#### Modules

##### Props

- Fixed a typo in one of TiledWallData's properties. renamed 'heigth' to 'height'.

##### User

- Fix connecting a signal to a non existent method in UserManagerDB.
- Update logic in UserManagerDB, as the default user id is -1 now.

##### Database

- QueryBuilder - Fix potential infinite recursion. 
- QueryBuilder  various small fixes
- TableBuilder - Fix missing default parameter. 
- Use String length() instead of size() in QueryBuilder, also guard against overindexing.

##### Database SQLite

- QueryBuilder, sql3QueryBuilder  various small fixes
- Set sqlite to serialized mode. 
- Make sure escape is used in SQLite3QueryBuilder wherever it's expected. 

##### Web

- Use String.length() instead of String.size() in WebServerRequest::setup_url_stack().
- Ensure a '/' in between the host, and the url in WebServerRequest::get_url_site().

### Changed

- Added custom_modules folder to the .gitignore.

#### Modules

##### Paint

- Full overhaul of the paint module.

### Removed

- Removed the unused libsimplewebm.

#### Modules

##### Database

- Removed a few unimplemented methods.

##### Web

- Removed old unneeded c++ files.


## [3.9.0] 

### Added

#### Backports

Backported everything up to and including https://github.com/godotengine/godot/commit/6812e66f33761ee4d02a7439af8fbea84e63e3f9 Merge commit: https://github.com/godotengine/godot/commit/3c39bc365fbfeda805aa87687422e9c9092c809b

#### Core

- Now the Engine has methods to store globals (custom singletons). They are mostly meant for classes like EditorInterface, SpatialEditor, and plugin scripts that need a singleton for in-editor use. In essence it provides easy and side effect less access for classes that might not be present on every run. (For example gdscript recognizes normal singletons as keywords, but not these.) Also registered EditorInterface, SpatialEditor, CanvasItemEditor, and WebNodeEditor as globals.
- Added msvc versions of the no-rtti and no-exceptions options. (They are disabled temporarily)
- Use FALLTHROUGH macro instead on [[fallthrough]]; 

#### Engine

- Added a new World class and made Viewport inherit from it. Moved World2D and World3D related things to World from Viewport. Also added the ability for overriding Worlds. Still needs some work, it should be already usable. (Sometimes when changing world overrides, a few camera related errors are emitted.)

#### Editor

- Now menu items can be added to the convert menu in the editor.
- Added an option to automatically reload or save instead of asking what to do after a file changed outside the editor.

#### Modules

##### GridMap

- Added back the GridMap module. Also added back support for it in other modules.

##### ThreadPool

- Now the target fps is customizable for ThreadPool when threading is not enabled.

##### Web

- Small improvements to the simple web server.
- Added enctype and multipart form helpers to HTMLTag.
- Added rows and cols to HTMLTag.
- Added proper multi part form parsing support for the simple web server. 

#### Releases / Builds / CI

- Updated the build containers from upstream.
- Added a strip script. 
- Now the podman build all script also builds armhf x11 executables.
- Added a script for engine side release preparation.
- Now the podman build all script has the ability to easily set the build name.
- Added a new command line option to help with generating version.txt for tpz files.
- Windows x86 github action.

### Changed

#### Core

- Added missing projection * projection Variant operator.

#### Engine

- Added _3d postfix to the end of the World3D related getters and setters in Viewport.
- Renamed Environment to Environment3D, and WorldEnvironment to WorldEnvironment3D.
- Renamed World to World3D. 
- Now Camera2Ds handle being current properly, and also more intuitively. 

#### Platforms

- Updated frt2 to the latest.

#### Modules

- Cleaned up old _PRESENT defines, now everything uses module configs.

##### TileSet

- Renamed RTileMap ro TileMap and RTileSet to TileSet, as it does not need to be able to live alongside Godot's TileMap anymore. Also added compatibility classes.
- Added back TileSet export.

##### GridMap

- Now MeshLibraryEditorPlugin registers itself to the export as menu. 

##### GLTF

- Moved the gltf export to the convert scene menu. 

##### ThreadPool

- Now ThreadPool should properly handle when use_threads changes. 
- Moved ThreadPool to core. Also it can change it's thread count now when it has time for it.
- Now ThreadPool uses a List internally as a queue instead of a Vector. 

##### Paint

- Added a few getters and setters to the PaintWindow. 

##### Skeletons

- Moved Skeleton and it's related classes to a module. 
- Moved Skeleton2D and it's related classes into a module. 

##### GDScript

- Now the return value discarded warning is disabled by default in gdscript and cscript.

##### Web

- Renamed WebEditor* to WebNodeEditor*, made it a singleton, and added an extension api for it. 

#### Build System

- Now the project setup script's module folders are customizable.
- Updated the example build config file. 
- Add quotes when passing the custom module folders parameter. 
- Now the app project helper build script supports custom module folders.

#### Releases / Builds / CI

- Now export templates are created in the .tpz format for easy installation. (Note that you can rename them to zip, and unpack them yourself if you need / want.)
- Replaced the default project editor icon, and the logo editor icon.
- Pass all arguments into the build commands in the podman build all file instead of the hardcoded -j4.
- Now versions will be handled differently, unlike in godot, I'll commit version information to the repository.
- Update hungarian translation from godot. 

### Fixed

#### Core

- Now String and CharString Types properly move the null terminator on resize and remove.

#### Editor

- Now all gizmo plugins return gizmo names properly. 
- Get gizmo names using the proper method for the toggle popup in the SpatialEditor.
- Fix errors on new scene creation.
- Remove unused file selector from EditorNode.

#### Modules

##### Voxelman

- Fix voxelman's editor setting. (Now it's under the Voxelman tab instead of under the Voxel tab.)

##### PluginRefresher

- Fixed PluginRefresher when a project only has one addon.

##### ThreadPool

- Fix race condition in threadpool.
- Fix setter parameter types for ThreadPool. 

##### Pain

- Improved PaintWindow mouse emulation from touch support a bit more. 
- Fix PaintWindow with mouse emulation from touch. 

##### Web

- Fix requesting write locks in WebNodes. 
- Fix default argument for the binding of WebServerRequest::send_redirect. 
- Fix: WebPermission was meant to be a Resource.
- Fix mimetype handling in the simle server. Also added a few missing ones.
- Fixed rendering the menu of the BrowsableFolderServeWebPage and StaticWebPage.

#### Build System

- Don't try to include x86 assembly into non-x86 based platforms when cross compiling the x11 platform on x86.
- Fix build when 3d is off. 

#### Releases / Builds / CI

- Don't remove the module config file in the build all script anymore.
- Fix the job name of the android editor's github action.

## [3.8.0]

### Added

#### Engine

Backported everything up to and including https://github.com/godotengine/godot/commit/989d5990ad704fe97a341cb5883d4725479ea09b

##### Core

- Added a new templated WeakRef (WRef) class.
- Added hash_set from godot4.
- Added RBMap and RBSet from godot4.
- Backported helper classes to pair.h from Godot4. 
- Ported the improvements to the hash funcs from Godot4.
- Backported the improvements to the Math class from Godot4.
- Added Vector4, Vector4i, and projection classes from Godot4.
- Backported almost all improvements to core math classes from Godot4. Also bound all eligible methods.
- Now Vector4, Vector4i, Projection, PoolVector4Array, PoolVector4iArray, are built in variant types. Also reordered the Variant's Type enum, renamed - _RID in it to RID, fixed a few smaller issues and removed some very old compat code.
- Backported the improvements to StringName from Godot4.
- Backported convert_rg_to_ra_rgba8 and convert_ra_rgba8_to_rg from Godot4's Image.
- Added more helper methods to DirAccessRef and FileAccessRef. Also smaller cleanups.

##### SceneTree

- Backported from Godot4: Node3D gizmo improvements, including subgizmos.
- Backported: returning AfterGUIInput from forward_spatial_gui_input from Godot4. Also removed the first index parameter.
- Renamed AFTER_GUI_INPUT_DESELECT to AFTER_GUI_INPUT_NO_DESELECT.
- Backported: add viewport.get_camera_2d()

##### Editor

- Now full screen editor plugins have the ability to hide their tab button in the top bar.
- Now the editor won't treat a hidden main button as an indicator that their editor plugin is disabled. This behavior was used to be a part of editor feature profiles, which I removed a long time ago.

##### Nodes

- Backported ImporterMesh and ImporterMeshInstance3D from Godot4. 
- Backported ImmediateMesh from Godot4.

##### Skeletons

- Backported some of the improvements to BoneAttachment from Godot4.
- Backported most improvements to Skeleton from Godot4.
- Backported: "Remove animation 3D transform track, replace by loc/rot/scale tracks" from Godot4.
- Backported Godot4's skeleton editor.
- Backported from Godot 4: New and improved IK system for Skeleton3D.
- Backported from Godot 4: New and improved IK system for Skeleton2D.
- Ported from Godot4: Remove REST transform influence in skeleton bones. (This means that: Animations and Skeletons are now pose-only. Rest transform is kept as reference (when it exists) and for IK. Improves 3D model compatibility (non uniform transforms will properly work, as well as all animations coming from Autodesk products).)

##### Networking

- Removed rsets.
- Moved visibility rpcs (vrpc) from Entity to Node. Also added an unreliable variant.
- Added a few small networking related helper methods to Node. Made the rpc macros in Entity use them.
- Removed deprecated enum values from multiplayerPeer.
- Removed additional direct script rpc mode query when sending rpcs, in order to simplify that codepath as much as possible. Also removed the remote, remotesync etc. keywords from gdscript, as now they won't work anymore. Node's rpc_config() method should be used instead of marking methods with keywords in scripts.

#### Modules

- Backported: [Scons] Implement module dependency sorting.
- Moved http server simple to it's own module.
- Moved the editor only modules to a new editor_modules folder.
- Removed stub module.

##### TextEditor

- Now text files will show up in the editor if the text editor plugin is enabled. Also clicking them will open them in the text editor.
- Now custom file creation entries can be added to the editor's FilesystemDock.
- Now the TextEditor addon will add it's own "create file" entry to the editor's filesystem dock.
- Now the TextEditor won't open a file multiple times, instead it will just switch to the proper tab.

##### Web

- Added web node editor plugin.
- Added icons for web nodes.
- Also created full documentation for most of the classes in the web module, and added descriptions and brief descriptions everything, except a few small helper utilities.

##### Users

Added icons for User, and user web pages.
Added icons for the UserManagers.
Added an icon for UserModule.

##### MeshDataResource

- Added an icon for MeshDataResource, MeshDataInstance, and for the meshDataInstance editor toggle.
- Now in order to edit MeshDataInstance a new edit mode has to be togged from the top bar. While this mode is enabled the edited MeshDataInstace can't be deselected. Also now Mesh Data Editor's sidebar will only be visible in this mode, along with the editor gizmo.
- The transform gizmo is hidden now while editing a mesh data resource.

### Changed

- Renamed Quat to Quaternion for consistency with the other engine math classes.
- Removed CameraMatrix, and switched to Projection.
- Now all Variant math types are structs.
- Ported form godot4: Reformat structure string operators.
- Backported from Godot4: Cleaned up Hash Functions. Clean up and do fixes to hash functions and newly introduced murmur3 hashes.
- Restructured the core folder's directory.
- Added _theme_ infix to the theme helper methods in Control.
- Renamed the VisualServer to RenderingServer like in Godot4, as I think it's a lot better name for it. I did not yet rename the files.

### Fixed

- Actually check whether modules are enabled, not just for their existence in mesh_data_resource's SCsub.
- Check whether the gltf module is enabled or not before trying to use it.
- Main build script: Properly set module_ name _enabled in environment to false for modules that get disabled using can_build().
- Fix serializing variants into binary files.
- Use AnimationPlayer's root property instead of just getting it's parent when exprting a GLTFDocument.
- Use string length() instead of size() as it was intended in HTMLParser, and BBCodeParser.
- HTMLParser now properly parses doctype in a case insensitive way. Also improved an error message a bit.

### Removed

- Removed AnimationTreePlayer, as it was deprecated (and also practically unusable).


## [3.7.0]

### Added

#### Modules

- Ported my web framework setup from rcpp_framework to a new web module, alongside with lots of improvements to make it fit the engine better. Also implemented a simple webserver that uses built in sockets.
- Ported my database setup from rcpp_framework to a new databases module, alongside with lots of improvements to make it fit the engine better.
- ported my user management setup from rcpp_framework to a new user module, alongside with lots of improvements to make it fit the engine better.
- Created a new cscript module, it's currently only a simplified gdscript implementation.
- Simplified, refactored and ported https://github.com/fenix-hub/godot-engine.file-editor to an engine module. (The refactered gdscript codebase is here: https://github.com/Relintai/godot-engine.file-editor )
- Created a new plugin_refresher module. It can be used to quickly enable / disable addons. You can enable it in the ProjectSettings->Plugins tab. Right click the refresher icon to select the plugin that you want to refresh, left click the same icon to actyally refresh it.

##### props

- TiledWalls got a new collider_z_offset property, which allows you to offset the generated collider shape.
- TiledWalls also got a new texture_scaling property. 

### Other

- Added a new property hint (PROPERTY_HINT_BUTTON) that can be used with NIL properties to add buttons to the inspector.
- Lots of TODOs.

#### Ports

- Ported all commits that I thought would be useful since the last set of backports from upstream godot 3.x.

### Changed

- Disabled nullptr modernization in clang tidy.
- updated the build container scripts.

### Fixed

- Lights.
- Codestyle in a few files.

#### Modules

##### entity_spell_system

-  Fixed the property validation for aura triggers and aura stat attributes in Spell.

##### props

- Fixed the 2 add methods in TiledWallData.

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
- [[Mesh Utils]](https://github.com/Relintai/mesh_utils) - A c++ Godot engine module containing utilities for working with mehses (Like simplification).
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

- UWP (I was never once able to successfully set up the environment for it to compile during the last 3 years).

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

- Removed TMeshInstance, and changed the core MeshInstance to work the way I need it instead.

#### other

- Removed the github issue templates.

### Fixed

#### Modules

- Added material cache support for Voxelman, and Terraman's bake texture mesher step type.

#### Editor Docs

- Fixed all warnings.

#### Editor Docs

- Limited the length of the default parameter in the class docs. This fixed the properties display for classes like Entity.

#### Other

- Github actions have been simplified and fixed.
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
