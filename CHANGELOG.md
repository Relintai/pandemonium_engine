# Changelog

All notable changes to this project will be documented in this file.

## [Master]

No changes yet.

## [4.0.0]

### Added

#### Core

- Bind RWLock, so it's accessible to scripts.
- Implemented a new ProcessGroup Node. It can be used to multithread scenes updates.
It was insipred by godot 4's ProcessGroup system, however while Godot 4's implementation tries to hide threaded processing as much as possible, this implementation focuses on making it explicitly known and obvious to the user, in a (hopefully) almost bolierplate free way. Also with the available options this node can be used for other purposes, like multi threaded cron job like method calls when paired with a Timer in manual mode.
- Ported parts of: Refactor Node Processing * Node processing works on the concept of process groups. * A node group can be inherited, run on main thread, or a sub-thread. * Groups can be ordered. * Process priority is now present for physics. This is the first steps towards implementing godotengine/godot-proposals#6424. No threading or thread guards exist yet in most of the scene code other than Node. That will have to be added later. - reduz https://github.com/godotengine/godot/commit/98c655ec8db17e50afa58284b1dcad754034db4b - Only got the smaller improvements, and the thread safety for Node and SceneTree. I'm planning to implement a similar system, but I have a different way of doing it in mind.

#### Modules

##### CSG

- Added back the csg module from godot.
- Ported the navigation geometry parsers for the csg module.
- Only build CSGGizmos when building the editor.
- Added back CSG support code I removed.

##### Web

- Implemented custom response headers for WebServerRequest.

##### GDNative

- Added back gdnative as a built in module. It was worked on in this repository: https://github.com/Relintai/gdnative .

### Fixed

- Fix binding inconsistencies.
- Fix property bind inconsistency.
- Fix property binding hints.
- Fix property types. Auras have been merged into Spells a while ago.
- Fix some property names being the same as getters / setters.
- Fixed disrepancies between a few getter and setters.
- Bind missing enum values.
- Fix duplicate parameter names in binding.
- Fix the order of some default arguments in bindings.

#### Core

- Fix building with ptrcall on.
- Fix the return type of PoolVector::count().
- Added grow_by and to_rect2 helper method to Rect2i.
- Now Basis::get_uniform_scale() returns a real_t instead of a float.
- Mark the parameters of Basis::rotate_to_align as const ref.
- Apparently Basis::rotate_to_align had a bug. Ported the fix by lyuma from: https://github.com/godotengine/godot/pull/77469/commits/6dfa6fc50e957dcffd6b2c33cc5a88448918d8a5
- Fix error, also add linear_interpolate call for Vector4i to variant op.
- Fix small inconsistencies in Vector3/3i/4/4i apis.
- Also bind Vector3i set_all().
- Added set_all() to Vector3i.
- Made the parameter of snap and snapped in Vector3 const reference.
- Make the parameters of String's and PoolVector's join() const ref.
- Fix handling dots in String::is_numeric().

#### Platforms

##### Android Editor

- Implemented switching between the running game and the editor in the android editor.
- Fix debugging on the android editor.
- Added warning for the android editor that android will kill the game after a few seconds if it's opened in the same window as the editor and then it goes to the backgond. Should be still enough to see what's the issue of you don't intentionally debug break though.

##### Windows

- Update the variant types in the windows natvis file.

#### Modules

##### Skeleton3D

- Fixed SkeletonModification3DFABRIK.

##### FastNoise

- Fix the type of FastnoiseNoiseParams::get_fractal_octaves().

##### Entity Spell System

- Fix virtual method names in Entity.

##### GLTF

- Fix registering types in the gltf module.

#### Docs

- Re-extracted class docs.
- Fix godot4 syntax in docs.
- Add missing classes to the editor_code_editor module's class list.
- Fix lots of small issues with the docs.

### Changed

#### Core

- Merged the functionality of BSInputEventKey to InputEventKey. This new setting can make input event keys act as if they are shortcut matched.

##### ThreadPool

- Removed _execute method bind from the NavigationMeshGenerator jobs.
- Don't print an error in ThreadPoolJob's _execute, as it's probably more intuitive this way.
- Don't bind _execute() method in ThreadPoolJob derived classes, as it's not needed anymore.
- Emit the completed signal automatically in ThreadPoolJob.
- Added virtual _execute to threadPoolJob.

#### Modules

##### Navigation

- Backported lots of improvements to the Navigation Servers from Godot 4. See the list in the Backports section.
- Renamed neighbor_dist properties / getters / setters for the navigation server.

##### MeshDataResource

- Use the new scaled grab radius in MDIGizmo.

##### Entity Spell System

- Move the enums from the top of spell.h into the Spell class.
- Added code to load old resource properties where applicable, to help converting resources to the new fixed apis. These will be removed after the next release.
- Renamed get_stat, and set_stat in Entity. It's just a smell c++ side helper method, they aren't bound. Should not require any changes like the rest.
- Fix Entity's api's inconsistencies.

In projects mass replacing the following words will update everything:

gets_is_pet -> pet_gets_is\
getc_is_pet -> pet_getc_is

gets_original_entity_controller -> original_entity_controller_gets\
sets_original_entity_controller -> original_entity_controller_sets\
gets_entity_controller -> entity_controller_gets\
sets_entity_controller -> entity_controller_sets\
getc_entity_controller -> entity_controller_getc\
setc_entity_controller -> entity_controller_setc\
gets_ai -> ai_gets\
sets_ai -> ai_sets

free_spell_points_gets -> spell_points_gets_free\
free_spell_points_sets -> spell_points_sets_free\
free_spell_points_getc -> spell_points_getc_free\
free_spell_points_setc -> spell_points_setc_free

gets_free_class_talent_points -> class_talent_points_gets_free\
sets_free_class_talent_points -> class_talent_points_sets_free\
getc_free_class_talent_points -> class_talent_points_getc_free\
setc_free_class_talent_points -> class_talent_points_setc_free\
gets_free_character_talent_points -> character_talent_points_gets_free\
sets_free_character_talent_points -> character_talent_points_sets_free\
getc_free_character_talent_points -> character_talent_points_getc_free\
setc_free_character_talent_points -> character_talent_points_setc_free

gets_bag -> bag_gets\
sets_bag -> bag_sets\
getc_bag -> bag_getc\
setc_bag -> bag_setc

sets_ai_state -> ai_state_sets\
ai_state_sets_stored -> ai_state_stored_sets\
ssend_stat -> stat_ssend\
creceive_stat -> stat_creceive\
getc_state -> state_getc\
setc_state -> state_setc\
gets_state -> state_gets\
sets_state -> state_sets\
adds_state_ref -> state_ref_adds\
removes_state_ref -> state_ref_removes\
gets_free_spell_points -> free_spell_points_gets\
sets_free_spell_points -> free_spell_points_sets\
getc_free_spell_points -> free_spell_points_getc\
setc_free_spell_points -> free_spell_points_setc\
gets_target -> target_gets\
sets_target -> target_sets\
getc_target -> target_getc\
setc_target -> target_setc

Mass replace these in the order given:

get_character_skeleton_path -> character_skeleton_path_get\
set_character_skeleton_path -> character_skeleton_path_set\
get_character_skeleton -> character_skeleton_get\
set_character_skeleton -> character_skeleton_set\
gets_entity_data -> entity_data_gets\
sets_entity_data -> entity_data_sets\
getc_entity_data -> entity_data_getc\
setc_entity_data -> entity_data_setc\
entity_data_gets_id -> entity_data_id_gets\
entity_data_sets_id -> entity_data_id_sets\
entity_data_getc_id -> entity_data_id_getc\
entity_data_setc_id -> entity_data_id_setc\
entity_data_gets_path -> entity_data_path_gets\
entity_data_sets_path -> entity_data_sets_path\
gets_ai_state -> ai_state_gets\
ai_state_gets_stored -> ai_state_stored_gets

##### Web

- Renamed the object() method in HTMLBuilder as it can cause issues with bind generators.

### Removed

#### Core

- Removed the Math singleton. If needed it can easily be recreated as a module.

### Backports

- Backported everything up to and including https://github.com/godotengine/godot/commit/43e181a00ab39671a3bb0b2d0302e25313f18aef Merge commit: https://github.com/godotengine/godot/commit/ac5d7dc82187940a5fb2908e276cf8eb0861cac4

#### Godot 3.x

- Single Compilation Unit build. Adds support for simple SCU build. This speeds up compilation by compiling multiple cpp files within a single translation unit. - lawnjelly https://github.com/godotengine/godot/commit/43e181a00ab39671a3bb0b2d0302e25313f18aef
- Canvas item hierarchical culling Adds optional hierarchical culling to the 2D rendering (within VisualServer). Each canvas item maintains a bound in local space of the item itself and all child / grandchild items. This allows branches to be culled at once when they don't intersect a viewport. - lawnjelly https://github.com/godotengine/godot/commit/b777a9e5f9838a98a0dc4c73f3ee6d777a5ab53d
- Revert "Add option in VisibilityEnabler2D to hide the parent for better performance"
This reverts commit ad6e504a5bc944892e93baacb5d9ee7de7f92e80.
- Add parameters for the Godot Activity starting intent to allow restarting or force-quitting the engine Follow-up code cleanup for #78130 - m4gr3d https://github.com/godotengine/godot/commit/5cf0ba88e32cecefcaa73b9e326c948412da86e0
- Expose the TextEdit control of the script editor
Refactor ScriptEditor and ScriptTextEditor with added method to retrieve the script editor's TextEdit control. - jeronimo-schreyer https://github.com/godotengine/godot/commit/a248c318de3f221d174becd39050ce0728d9caf0
- Move autotile fallback helper functions + fix comments and docs - wareya https://github.com/godotengine/godot/commit/a40ecc71e65eaf00895b4bd01db0495664be4274
- Make autotiles fall back to the most similar bitmask using heuristics - wareya https://github.com/godotengine/godot/commit/73ad6517e4f5344f909a58dff7347eb69fe0a539
- Fix issue causing the last edited project to open while switching to another one. Fixes #76562 -m4gr3d https://github.com/godotengine/godot/commit/ee07f60b07617fb470ec74294b5148a0dd8ec3aa
- Fix spatial viewport multitouch detection support Regression introduced by #77497 - m4gr3d https://github.com/godotengine/godot/commit/8f447658311451985951157ac6284f0b36e4d9d0
- Input - fix just pressed and released with short presses Previously if an action was both pressed and released on the same tick or frame, `is_action_just_pressed()` would return false, resulting in missed input. This PR separately the timestamp for pressing and releasing so each can be tested independently. - lawnjelly https://github.com/godotengine/godot/commit/63d208d1b0d9bf92f07dcd41a7d7a1c7d8baa8d4
- Added missing descriptions to Input's class docs from godot.
- Update gamepad button/axis names to match 4.0
- Add example for `NodePath` to grandparent
- Clarify `String.get_slice` behavior Clarify that the function returns the whole string if there is no instances of the delimiter in the string.
- Fix overwriting of Spatial's local transform Modifies when 'DIRTY_LOCAL' flag is set to prevent a transform applied using `set_transform` to be overwritten by previous calls to change the node's rotation, translation or scale. Fixes #43130.
- Backport VideoLooping and fix for initial black frame
- Bounds fixes in `TextureAtlas` import
- Use current keyboard layout in OS_X11::keyboard_get_scancode_from_physical.
- Fix issue causing the Android editor to crash when creating a new AudioStreamMicrophone Fixes #73801 - m4gr3d https://github.com/godotengine/godot/commit/9c334fa242b35c2aa332556699cb094954a958d1
- Improve touchpad and mouse support for the Android editor - m4gr3d https://github.com/godotengine/godot/commit/ccd36e0dbec048ef1645691d0cf838465bfb4bd0
- Adds a scale_gizmo_handles entry to the Touchscreen editor settings When enabled, this scales the editor icons to improve usability on touchscreen devices. In addition this commit fixes touch detection for the collision_shape_2d_editor_plugin so it scales with the icons size. - m4gr3d https://github.com/godotengine/godot/commit/b78935ef518ae5a63451d6de32f9df6b35eb8cde
- Enable granular control of touchscreen related settings - m4gr3d https://github.com/godotengine/godot/commit/61e41facc7e7aceac0fcdb1b90e433566301098c
- Augment the InputEvent class with a CANCELED state The `InputEvent` class currently supports the `pressed` and `released` states, which given the binary nature, is represented by a `bool` field. This commit introduced the `CANCELED` state, which signals that an ongoing input event has been canceled. To represent all the states, the `InputEventState` enum is added and the `InputEvent` logic is refactored accordingly. - m4gr3d https://github.com/godotengine/godot/commit/94d6c3dcc68f9de4b7c004e3a18513edb1d5dda2
- Add setting to control the window used to run the project for the Android editor - m4gr3d https://github.com/godotengine/godot/commit/b5a908c985e4f3e5633751012fac64ab3ba0fbbf Removed my force launch adjacent solution in favor of this.
- Fix compilation of basis unit test - Calinou https://github.com/godotengine/godot/commit/d1c8c5dd304f82acbbeb903e4a8ec86a63a3930e
- Test, refactor and fix a bug in Basis.get_axis_angle - fabriceci https://github.com/godotengine/godot/commit/9f1a57d48b296c607b76fdae30a78630065710e6
- [3.x] Fix NODE_POSITION_VIEW Shader Built-In - paddy-exe https://github.com/godotengine/godot/commit/67d3fe4075955bd5eb8892c9610a3b469e81cbc9
- Backport spatial shader built-ins Backport of this PR: godotengine#63597 This adds these as new Built-Ins to Spatial Shaders * Object's Position in World Space * Camera Position in World Space * Camera Direction in World Space * Object's Position in View Space - paddy-exe https://github.com/godotengine/godot/commit/be3d331f263a8ad1900f7b31965ff2d25f73c2e9
- Follow up to https://github.com/godotengine/godot/pull/76400 to fix input ANR in the Godot Android editor
- Allow concurrent buffering and dispatch of input events
- Fix trim when importing WAV
- Fix PopupMenu's maximum height not being automatically set
- Cache text property when toggling BBCode
- Add Tab Metadata
- Improve tooltip for CanvasLayer.layer (3.x)
- Fix small mistake in docs, "antecedents" -> "ancestors"
- Add allow_search property to ItemList and Tree
- Clarify range of various ID values are 32 bit
- Stop dragging when a slider changes editability
- Warn against using non-uniform scale for 3D physics (in class reference only) partial backport of #67847 fixes #56824
- Minor improvements to NetworkedMultiplayerCustom.xml
- Fix docs on multiplayer peer signals.
- Fix `NodePath` subname index range documentation
- Linux: Don't use udev for joypad hotloading when running in a sandbox
- Add unsafe version from asin and acos.
- Make acos and asin safe
- update mymindstorm/setup-emsdk to v12
- Document 3D particle color properties requiring vertex color as albedo - Calinou https://github.com/godotengine/godot/commit/c65967c17f30f94568396a620249df5a93a75872
- Multirect - Fix refining regions for all derived Textures Fixes allowing all derived texture types to modify region prior to rendering. - lawnjelly https://github.com/godotengine/godot/commit/43b6205887605c7ac2ac79d4bf9306fb143abc80
- Fix rendering tiles using nested AtlasTextures - kleonc https://github.com/godotengine/godot/commit/12c923cb8b19ca9dc49c45601e8e20630cbd8497
- Fix GridContainer max row/column calculations not skipping hidden children - kleonc https://github.com/godotengine/godot/commit/0ce6ef72156291bdd4993dd0d6f6bf2b794c0baf
- Fix size error in `BitMap.opaque_to_polygons` Previous estimate of upper limit on size was incorrect
- Allow concurrent buffering and dispatch of input events

#### Godot4

- Support threads in the script debugger * This implementation adds threads on the side of the client (script debugger). * Some functions of the debugger are optimized. * The profile is also now thread safe using atomics. * The editor can switch between multiple threads when debugging. This PR adds threaded support for the script language debugger. Every thread has its own thread local data and it will connect to the debugger using multiple thread IDs. This means that, now, the editor can receive multiple threads entering debug mode at the same time. - reduz PR 76582 Will be available here after it's merged: https://github.com/godotengine/godot/commit/6b176671c4289e9096ea94583fae35f2c4686b7c
- Add Navigation Debug for GridMap edge connections Adds navigation visual debug for GridMap edge connections that use baked navigationmesh with bake_navigation=true. - smix8 https://github.com/godotengine/godot/commit/4f0730d4bcad43e73c39f7d77f5bef5f2215440a
- Rework Navigation Avoidance Rework Navigation Avoidance. - smix8 https://github.com/godotengine/godot/commit/a6ac305f967a272c35f984b046517629a401b688
- Rework const on NavigationServer methods `const` is used on all methods, even when they cause modification of the server.  This reworks the methods of the server to only use `const` on method that don't change the state of the server. - DarkKilauea https://github.com/godotengine/godot/commit/a0715b30f94430078422ff05d81b9cfa85ddb4ce
- Make navigation mesh edge connections optional Makes navigation mesh edge connections optional. - smix8 https://github.com/godotengine/godot/commit/f986b52b3cc107374d4e74774c8695a0f1282e11
- Add NavigationLink helper functions for global positions Adds helper functions to set the links start and end position with global positions or get them as global positions. Adds global start and end position for the navigation link to the 'link_reached' signal of NavigationAgent. That signal gets emitted when a navigation link waypoint is reached. Requires that 'owner' meta data is enabled on the NavigationAgent. - smix8 https://github.com/godotengine/godot/commit/d87f1247689ae82996aeac77b6e9870bbc88142d
- Enable assigning an owner to navigation regions and links This allows users of the server APIs to get back the nodes that created certain regions and links. - DarkKilauea https://github.com/godotengine/godot/commit/5769b0e8d8db75ff500c979aa63d3f3428de526d
- Add support for emitting a signal when entering a NavLink - DarkKilauea https://github.com/godotengine/godot/commit/5d8ba2b2d11fc6c4debdf21fa91bddefaa6f3d6d
- Prevent unnecessary navigation map synchronizations Prevents unnecessary navigation map synchronizations triggered by redundant calls to setters of e.g. region, link or map properties. - smix8 https://github.com/godotengine/godot/commit/7e1a261cc661ddbefb90208b94a6828c50750237
- Update NavigationAgent to use query_path This paves the way for having agents respond to link traversal. - DarkKilauea https://github.com/godotengine/godot/commit/a2c53b881b1a54c95036f27577ee7d9b6e583d62
- Add navigation layer bitmask helper functions Adds helper functions to work with the navigation layer bitmask. - smix8 https://github.com/godotengine/godot/commit/55923ade68237ae6a344efe23c2656ed7ba976c2
- Add NavigationAgent Path Debug Visualization Adds path debug visuals for NavigationAgent2D, NavigationAgent3D and NavigationServer. - smix8 https://github.com/godotengine/godot/commit/0ab764e84bc9d7f21292f954fb2be215377a7276
- Fix NavigationMesh debug visuals for non-triangulated meshes Fixes NavigationMesh debug visuals for non-triangulated meshes. - smix8 https://github.com/godotengine/godot/commit/4490a3303bc8b234dcccdbfdafd3877c9a11cb4d
- Fix 2D navigation debug visuals ignoring half the ProjectSettings Fixes that NavigationRegion2D and TileMap debug visuals ignored more or less half the ProjectSetting. E.g. random color could not be disabled, edges did not display. https://github.com/godotengine/godot/commit/2b19c70664dafd3e5689fc612feb7f7ac17c1a0a
- Rename Navigation uses of 'location' to 'position' Contrary to the entire rest of the engine NavigationAgent's and NavigationLinks decided to deal with locations instead of positions. - smix8 https://github.com/godotengine/godot/commit/bf1571979cd3cd90b2b5c4581b2947450aa4cd9d
- Add NavigationServer Performance Monitor Adds Performance Monitor for NavigationServer3D. - smix8 https://github.com/godotengine/godot/commit/9802914f9793b6888cc70e3d7f0d815bdd5188bb
- Add NavigationServer2D debug functions Adds debug functions to NavigationServer2D to mirror NavigationServer3D functions for 2D users. - smix8 https://github.com/godotengine/godot/commit/d254f0fa5f2fe23c4746b5b5a0f0e44f8613a0eb
- Fix navigation debug not toggleable in scripts Fixes that navigation debug was not toggleable in script while even the docs mentioned it. - smix8 https://github.com/godotengine/godot/commit/aecad7bb25bf9b13c0ddc30a91cb68787c8dd53b
- Fix Editor Navigation debug edge connection visuals Fixes missing Navigation debug edge connection visuals in Editor due to disabled NavigationServer. - smix8 https://github.com/godotengine/godot/commit/8bfea7dcb46b0a566bf1de7caa40533f29774e91
- Fix NavigationServer internals still using float instead of real_t Fixes that some NavigationServer internals still used float instead of real_t in some parts. - smix8 https://github.com/godotengine/godot/commit/217a27014bb9e88605f44de3f0bb119f372a8378
- Added node for Navigation links . - DarkKilauea https://github.com/godotengine/godot/commit/3dd59013f45b84cd0ded147df7684ffab424e407
- Add NavigationPathQuery Adds NavigationPathQueryParameters objects that can be used with NavigationServer.query_path() to query a customized navigation path. - smix8 https://github.com/godotengine/godot/commit/63dcb9aa80a2c77053033ed3c39b4fe5ed6f229b
- Fix NavigationRegion3D gizmo's odd visual behavior . - DarkKilauea https://github.com/godotengine/godot/commit/92c40bcf325ce91279c14f0b6a42c200fe4faf51
- Remove / Replace old Navigation Debug Visualization - removes / replaces leftovers from old navigation debug code - cleanes SceneTree and ProjectSettings from old navigation debug - smix8 https://github.com/godotengine/godot/commit/d7f75fab606fece5f7eb083ef6931f75815630fc
- Add more detailed Navigation Debug Visualization - Adds more customization options to ProjectSettings. - Displays navregion edge connections and navigation polygon edges in editor and at runtime. - Majority of debug code moved from SceneTree to NavigationServer. - Removes the irritating debug MeshInstance child node from NavigationRegion3D and replaces it with direct RenderingServer API. - smix8 https://github.com/godotengine/godot/commit/c394ea518e48bbce710b251cf20be078505ef8d7
- Fix NavigationMesh baking AABB Editor handling and visuals Fixes handling and visuals for Navigation Mesh baking AABB in the Editor. - smix8
- Implement NavigationMesh bake area. Adds two new properties to NavigationMesh resources to restrict the navmesh baking to an area enclosed by an AABB with volume. - smix8 https://github.com/godotengine/godot/commit/0c4d99f4fdcee4b1b6c289c83fb448262e60974b
- Rework NavigationMeshGenerator Reworks NavigationMeshGenerator and navigation mesh parse and bake process. Adds navigation mesh baking for 2D. - smix8 https://github.com/godotengine/godot/pull/70724 https://github.com/godotengine/godot/pull/70724/commits/38699a82593ef60d680b7b46b06c92f896dd7c77


## [3.11.0]

### Added

#### Core

- Added module initialization levels (similar idea to what's in godot4, although I added more).
- Backported the tight version of godot4's LocalVector as the new TightLocalVector class.
- Small cleanups for PooledList and PagedAllocator. 
- Renamed Map to RBMap.
- Renamed Set to RBSet.
- Replaced the HashMap's implementation with the one Godot4. Refactored it to work as a drop in replacement. Renamed the old one to OGHashMap.
- Added more getters to the new HshMap, for more backwards compatibility.
- Added the rest of the new container classes from godot4.
- Added utf8_byte_length(), and utf16_byte_length() helper methods to String.
- Added HAS_TRIVIAL_CONSTRUCTOR, HAS_TRIVIAL_DESTRUCTOR, and HAS_TRIVIAL_COPY macros to typedefs to fix new clang deprecations.
- Added Size and Point typedefs for Vector3i and Vector4i. 
- Added log10 to the Math class. 
- Added a Math singleton exposing math functions directly to scripts. The idea is to make the disrepancies between scripts and engine side code smaller.
- Added erf to Math. 

#### Servers

- Now more than one Navigation Server can be registered. 
- Move NavigationServer2d's forwarding logic to the navigation module as a new derived class.
- Now more than one Naviugation2D servers can be registered. 
- Allocate the selected navigation srevers. 
- Add init() virtual methods to the navigation servers. 

#### Editor

- Added a setting to force launch the project in an adjacent window in the android editor.
- Re-enable remote debugging in the android editor build.

#### Misc

- Added alternate slim args when building the editor with the setup script. Also updated the list and style of the example.

#### Platforms

#### Modules

- Added include guards to all module register_types.h-s.

##### Steering AI

- Added a new steering_ai module. It's a c++ port of https://github.com/GDQuest/godot-steering-ai-framework , with smaller modifications.

##### Web

- Added a new WebServerRequestScriptable class, so different web server request implementations can be created using scripts when needed.
- Added missing binds in FileCache.
- Added protocol error handling to the http server simple.
- Added optional protocol error logging to HTTPParser. Also remove unnecessary error.
- Set a better self signed cert path for the HTTPServerSimple.
- Pre-create the required folders for the self signed cert files automatically.
- Implement sending keep alive connection type if a connection has more than one request for HTTPServerSimple. Also more togglable debugging.
- Implement max request size limit for HTTPServerSimple. 
- Now post and get parameters can be set / changed in requests from scripts aswell.
- Added missing helper method to WebServerRequest. 
- Added more mimetypes to the HTTPServerSimple. 

##### UnitTest

- Added a new skeleton unit test module with some notes. 

##### EditorCodeEditor

- Move the editor's script text editor into the new code_editor module.

##### ShaderEditor

- Separated shader editor into a new module.

##### Props

- Added noise offset support to TiledWalls.

##### MaterialMaker

- Ported SlopeBlur from MaterialMaker. 
- Ported the TonesStep Node from MaterialMaker. 
- Ported the Warp Node from MaterialMaker.
- Added buttons slots to MMGraphNodes. 
- Ported the TonesMap from MaterialMaker. 
- Tones Node + Editor port from MaterialMaker. 

##### NavigationServerDummy

- Added a new dummy navigation server module. 
- Make sure the dummy navigation server is used automatically if the normal is disabled.

### Fixed

#### Core

- String: Use set_length, instead of resize in String's operator +=.
- Updated the logic of a few getters in String. 
- Fix warning on clang.

#### Editor

-  Fix selecting the script editor even if it's not the 3rd in the editor_tables array.
-  Make sure that the script editor is the 3rd tab.

#### Platforms

- Fix sprintf deprecation warning on osx.

#### Modules

- Now modules in custom folders can properly declare their copyright.txt.

##### Web

- Set _server_quit in WebServerSimple the way it was intended to be used.
- Fixed multi threading + https in HTTPServerSimple.
- Fixed more cases of using String.size() instead of String.length(). 
-  Fix Content-Length calculation in HTTPServerConnection::send when using non-ascii utf-8 characters.
- Fix handling utf-8 in the MarkdownRenderer. 
- Use the size struct member when parsing markdown in MarkdownRenderer. 
- Properly handle non-ascii characters in uris in HTTPParser. 
- Quick fix for accessing files in the pck file for FileCaches. 
-  Parse and process http headers in a case insensitive manner in HTTPParser.
- Fix string parameter parsing in multipart forms, also make multipart form content fields case independent.

##### Skeleton3D

- Fix error spam in the skeleton editor. 
- Make sure Skeletons set up their initial pose. 
- Cleanups to Skeleton's bindings. 


##### Entity

- Add braces around one liner if-s in Entity.
- Proper bind parameters for body_instance. 

##### MaterialMaker


- Fixed MMSdf3dOpExtrusion's name and added it to the build. 
- Reworked MMOutputImage. Now it uses a button, and also it works as it should.

##### Paint

- Fixed the tooltip of the Add Paint Canvas button of the PaintProjectToolsPropertyInspector.

##### MeshDataResource

- Reworked undo redo handling in the MeshDataResource Editor. Now it will properly mark the actual resource dirty in all cases. Also fixed other small inconsistencies, and issues.
- Fix visual indicator toggles in the MeshDataResource editor. 
- Also fix the tangent generation button in the MeshDataResource editor. 

### Changed

#### Modules

##### CVTT

- Moved the cvtt module to the editor modules folder, as it's tools only.

##### Entity

- Make body related methods in entity have body as their prefix. (For example: get_body_path() -> body_get_path()).
- Make body_instance in Entity virtual.

##### Props

- Don't serialize editor only omni lights in PropDataLight.

##### Web

- Improved PagedArticleWebPage's api.
-  Refresh the file cache in WebRoot on NOTIFICATION_READY. 

### Removed

### Core

- Removed the deprecated clamped() method from vector2. (The new method is limit_length().)

#### Modules

##### TileMap

- Removed RTileMap and RTileSet compatibility classes.

### Backports

#### Godot 3.x

Backported everything up to and including https://github.com/godotengine/godot/commit/cc60359a6c8b91d5df8c21f09f416b5d2ae87e94 Merge commit: https://github.com/godotengine/godot/commit/adee8cfee6df58744e08ccd03917829cc3faf465

- Tweak particles animation offset property hint to allow more precise values
- Fix Tree overflow without scrolling being enabled 
- Fix RichTextLabel discards appended BBCode text on window resize when using DynamicFont
- Fix local variables not showing when breaking on final line 
- Expose API to force file system sync. 
- Fix stylus tilt Y direction. 
- iOS: Add new model identifiers for DPI metrics 
- Support Git worktrees in generation of hash header 
- Fix get_path() is not working when files are opend with `open_compressed` And also fixed `get_absolute_path()` in the same way
- Make CollisionShape selection box use shape AABB 
- Bump json5 from 1.0.1 to 1.0.2 in /platform/web 
- zlib/minizip: Update to version 1.2.13, remove zlib from freetype Security update, fixes CVE-2022-37434 in zlib.
- Faster queue free
- Fix error when dropping script into script editor 
- Fix for PoolArray comparison - AThousandShips 
- Set touch input as handled only after _gui_call_input - necrashter
- GLES2 fix octahedral half float unpacking 
- Expose OS.read_string_from_stdin() to the scripting API This can be used in scripts to read user input in a blocking manner.
This also removes the unused `block` argument, which is always `true`.
- Fixed: Save Branch as Scene not workking in Remote Tree (only work when u pause)
- Suggest Filename in Remote Tree File Dialog 
- PopupMenu rework and enhancements Many scrolling behaviour improvements and the ability to limit popup size.
- Add dumb theme item cache to Control 
- Windows: Fix heap overflow setting native icon 
- Update the logic to calculate the screen scale on Android. Takes into account the ratio between the screen size and the default window dimensions.
- Improve error messages and classref for occluders and portals. Misused functions would previously produce no error messages which was confusing for users.
- Provide a delegate implementation for the killProcess logic on Android
- Add a theme usability setting which updates the touch area of UI elements (e.g: scrollbar) for the editor on touchscreen devices
- Add independent spinbox arrow step precision 
- Implement file provider capabilities. The previously used file sharing api was restricted after Android N causing the engine to crash whenever used on devices running Android N or higher.
- Document image size restrictions for custom mouse cursors in HTML5 
- Fix wrong SpriteFrames docs 
- Fix Line2D UVs when using BOX end cap mode 
- Default update_vital_only to true for Android and Web editor
- Fix the issue causing long-press on a selected node on the scene tree to trigger both the context menu and the rename functionality.
- Fix RichTextLabel: BBCode [color] tags are not counting in font char spacing
- Improve the clarity of Viewport's documentation 
- iOS: Fix memory leak on touch input
- Fix Editor hanging if audiostream's pitch_scale is NaN 
- TileSet: Fix resizing collision shape when vertex is outside the tilesheet
- BVH - fix lockguards for multithread mode 
- Add error messages for collision exception functions 
- Backport Tree::set_selected 
- Backport a simpler version of the accent color for check icons 
- Document using String.percent_encode() with OS.shell_open()
- Use the new API for virtual keyboard height detection on Android, bugfix .
- iOS: Implement missing gamepad.buttonOptions, buttonMenu, and buttonHome joy buttons.
- Fix AltGR getting stuck on Windows right alt-tab 
- Fix Xbox Series controller duplicate input 
- Mention String.match() is also called "glob"/"globbing" 
- Math: Prevent division by zero in posmod 
- Add PS3 controller guide button 
- Sync controller mappings DB with SDL2 community repo 
- Change message of unknown joypad property from error to warning. 
- Fix Standard Gamepad Mapping triggers. 
- increased max touches to 32 for ios 
- fix shadows pass viewport calculation. ([3.x] Fix shadows when using 2 directional lights)
- CI: Pin SCons to 4.4.0, the new 4.5.0 is broken 
- GDScriptParser - don't use index operator on linked list. 
- Use hash table for GDScript parsing
- Fix GridMap free navigation RID error spam 
- Set the unlit / unshaded extension when importing / exporting GLTF 
- Fix for 2D viewport not updating in the editor when the camera moves 
- Eliminate collision checks between geometry in rendering BVH. 
- Add set_value_no_signal() to Range 
- Add Color + alpha constructor for Color 
- Fix directory access when the running app has the All files access permission
- Bump the target SDK version to 33 (Android 13) 
- Update make_rst.py to match the master version 
- Make MessageQueue growable 
- Batching - Add MultiRect command 
- SurfaceTool - efficiency improvements 
- Fix null in android text entry system. 
- [3.x] Don't apply scale to autohide theme property 
- [3.x] Expose more compression formats in Image 
- Downgrade android gradle plugin to version 7.2.1. 
- Make tab's close button responsive to touch taps 
- Make EditorPropertyLayersGrid responsive to touch taps 
- Add option in VisibilityEnabler2D to hide the parent for better performance
- Backport from Godot 4 - Fix RigidDynamicBody gaining momentum with bounce
- Fix inconsistent file dialog settings usage 
- Suggest class_name in 3.x autocompletion. 
- MessageQueue - Fix max usage performance statistic 
- [3.x] Implement physics support in the GLTF module 
- Make create folder popup support nested folders 
- Fix Polygon2D skinned bounds (for culling) 
- Add benchmark logic


#### Godot4

- Convert syntax highlighters into a resource
- Extract Syntax highlighting from TextEdit and add EditorSyntaxHighlighter
- Changed line_edited_from(from) to lines_edit_from(from, to)
- Fix colour region continuation over blank lines, issue 41120
- Fix color region end key seach and start key order.
- Switch from recursion to iterative for backfilling colour regions.
- Fix highlight color for class attributes that are also keywords.
- Highlight control flow keywords with a different color. This makes them easier to distinguish from other keywords.
- Improve GDScript Editor and Improve latency
- Allow unicode identifier in GDScript syntax highlighter
- Rework how current Camera2D is determined
- Fix Camera2D crashes
- Also fixed issues with active Camera2D switching.
- Fix Viewport root order after Node2D raise
- Remove NOTIFICATION_MOVED_IN_PARENT
- Optimize Node children management
- Optimize Node::add_child validation

#### Godot plus

- Add ability to mute AudioServer.

## [3.10.0]

### Added

#### Backports

- Backported everything up to and including https://github.com/godotengine/godot/commit/0f10eafb38bd54e59fa7fd079ceafd2c07e8be2a 

#### Core

- Added support for specifying a COPYRIGHT.txt file for modules.
- Never print an error message in Object::get_meta(). 
- Remove now superfluos check from String's operator[]. 
- Add _FORCE_INLINE_ to String's resize(). 
- Added set_length helper method to String.

#### Editor

- Now EditorPlugins can register nodes to be shown on the `Create Root Node` tab.

#### Platforms

##### Android

- Now the desired android arches can be easily specified for the android build when using to the app build script.

#### Modules

##### Database 

- Added initialized signal and helper method to the DataBaseManager. It can be used to let automally allocated classes (singletons, main scene classes) in heavily database driven applications know when it's safe to use the db.
- Added docs for the QueryBuilder. 
- Added docs for the TableBuilder. 

##### User

- Added signals for user actions into the user web nodes.
- Added a simple page rendering customization api for UserWebPages.

##### Web

- Added HTTPSessionManagerDB using the disabled code in HTTPSessionManager, and cleaned up it's (and HTTPSessionManager's) logic.
- Added new tag() and ctag() universal tag helpers to the HTMLBuilder binder class.
- Also added tag() and ctag() helpers to the c++ side HTMLBuilder, and smaller cleanups.
- Added get request parameter support for WebServerRequest. 

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

##### Database

- QueryBuilder: Fix potential infinite recursion. 
- QueryBuilder: Various small fixes
- TableBuilder: Fix missing default parameter. 
- Use String length() instead of size() in QueryBuilder, also guard against overindexing.

##### Database SQLite

- QueryBuilder, sql3QueryBuilder  various small fixes
- Set sqlite to serialized mode. 
- Make sure escape is used in SQLite3QueryBuilder wherever it's expected. 

##### Props

- Fixed a typo in one of TiledWallData's properties. renamed 'heigth' to 'height'.

##### User

- Fix connecting a signal to a non existent method in UserManagerDB.
- Update logic in UserManagerDB, as the default user id is -1 now.

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
