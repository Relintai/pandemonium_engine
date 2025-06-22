# Changelog

All notable changes to this project will be documented in this file.

## [Master]

Nothing yet.

## [4.7.0]

Highlights:

- Added a new "Merge Back Changes" and "Merge Back Local Changes" tool when right clicking instanced scenes in the editor.
  When working with instanced scenes, these can be used to save changes back to the original scene file.
  The "Merge Back Local Changes" tool skips saving transforms.
- Created a new TOTP module. TOTP stands for Timed One Time Password.
- Full and improved docs for the entire fastnoise module.
- Full and improved docs for the entire lz4 module.
- Full and improved docs for the entire web module.
- Full and improved docs for the entire database and database_sqlite modules.
- Full and improved docs for the entire entity_spell_system module.
- Full and improved docs for the entire user module.
- Updated all links in the in-editor docs.
- Heavy iprovementst `FileCache`.
- Heavy iprovements to the `users` module.
- Added an editor gizmo plugin for TiledWalls and PropInstances so they can be selected in the spatial editor directly.
- TerrainWorld now has built-in support for loading and saving TerrainWorld chunks dynamically to and from disk.
- Added spawners (with editor and prop support) for the entity spell system: ESSEntityWorldSpawner3D, ESSEntityWorldSpawner3DSingle, ESSEntityWorldSpawner2D, ESSEntityWorldSpawner3DArea.

Breaking changes:

- Split  _create_chunk() to _create_chunk() and _setup_chunk() in TerrainWorld.
  Unfortunately this is a breaking change. (Very slight though.)
  Now _create_chunk() should be used to initialize serialized properties
  in chunks. The new _setup_chunk() virtual should be used to initialize
  other non/serialized properties (like meshers).
  Fortunately the only thing that needs to be done is to split old
  _create_chunk() into two.
  Note that when using procedural generation, the old way should just work
  without any updates necessary. This change is only needed when loading
  of chunks is desired.

c++ only:

- Now get_all_as_vector() in UserManager is not virtual anymore. Also removed other implementations.
  - This had to be done as get_all_as_vector() cannot be customized by
    scripts, and if an inherited UserManager needs to set up UserModules
    this will cause issues later down the line.
  - Other solutions like a user_setup() method could also work, but then
    that would cause more individual (non-optimizable) lookups to db-based
    backends (and likely others too), so this solution seems to be the best
    currently.

### Core

- Use SpinLock in GLOBAL_CACHED. Inspired by https://github.com/godotengine/godot/commit/187e14b8ac7e5508e87355bdb8d3d7a856f54e0b
- Update copyright information in main.

### Editor

- The "Access as Unique Name" tool option in SceneTreeDock now always have a separator.
- Move EditorPropertyRevert to it's own file.
- Added a new Merge Back Local Changes too when right clicking instanced scenes in the editor.
- Better implementation for the new "Merge Back Local Changes" tool.
- New alternate version of the "Merge Back Local Changes" tool which keeps transforms intact.
- Renamed the "Merge Back Local Changes" tool in the SceneTreeDock popup menu to "Merge Back Changes".
- Update links in the docs.
- Handle proxy classes properly in arguments and return values in EidtorDocs.
- Remove stray rset and rpc mode variables from gdnative.
- Removed stray rset_mode and multiplayer keywords from the docs.
- Removed more stray old multiplayer keywords from the docs.

### Scenes

- Fix crash in TextMesh.

### Modules

#### database and database_sqlite

- Implemented table version support for SQLite3DatabaseConnection.
- Removed unused include.
- Removed unnecessary overloads from QueryBuilder.
- Removed unused include.
- Removed unnecessary #if.
- Added alter table helper methods to TableBuilder.
- Split concatenations into multiple lines in SQLite3TableBuilder, since now TableBuilders use a StringBuilder this saves a little bit on concatenating strings.
- Added recommandations to PreparedStatement and QueryBuilder's docs.
- Added Database::get_backend_name() virtual method.
- Document set_table_version() being able to remove entries.
- Full and improved docs for the entire module.

#### gdscript

- Disabled integer division and unused signal warnings by default in gdscript.

#### web

- Hode session_id and id in HTTPSession.
- Added serialization helper methods to HTTPSession.
- Added a new HTTPSessionManagerFile class.
- Automatically load sessions when HTTPSessionManagerFile enters tree.
- Removed unnecessary c++ only method from FileCache.
- Made cache_invalidation_time member protected in FileCache.
- Use uint64_t directly in FileCache's cache_invalidation_time getters and setters.
- Fix cache_invalidation_time property name in FileCache.
- Use StringNames in FileCache caching functions.
- Use HashMap in FileCache instead of an RBMap.
- Do proper cleanup in FileCache destructor.
- Added clear_expired() helper method to FileCache.
- Tweaks to caches in FileCache.
- Don't use dynamically allocated CacheEntries in FileCache.
  - This simplifies the code a lot, also makes it harder to have
    threading-related issues.
  - Also fix potential race condition.
- Added new helper methods for folders into FileCache.
- Removed unused imports from FileCache.
- Tweak some of the variable names in FileCache.
- Renamed the path parameter for the cache methods in FileCache. It's much more useful thinking about them as keys instead of paths.
- Added more caching helper methods to FileCache.
- Actually return the absolute path in FileCache::wwwroot_get_folder_abspath().
- Close the file and don't leak memory in SimpleWebServerRequest::move_file() when in-memory uploads are used.
- Fix _validate() method's virtual bind for FormFieldEntry, FormField, and FormValidator.
- Reworked FormFieldEntries to use format strings.
- Fix data tag in HTMLBuilder.
- Implement HTMLBuilder::wr().
- Made the result variable in HTMLBuilder protected. It now has a getter.
- Use StringBuilder in HTMLBuilder in the background.
- Use empty() instead of checking against "" in HTMLBuilder.
- Use empty() instead of checking against "" in HTMLBuilder's bind class.
- Use StringBuilder in HTMLBuilder's binder class in the background.
- Finish the last tag for convenience before returning the result in HTMLBuilder.
- Fix unfinished methods in HTMLBuiler's binder class.
- Don't mess with the base_url by adding a slash at the end if it's missing in HTMLPaginator.
  This way usage logic should be simpler. It will also work with old style
  GET request parameters.
- Fix HTMLPaginator::start() and next() skipping the first page.
- Full and improved docs for the entire module.

#### users

- Fix calling the wrong method in UserManager.
- Now get_all_as_vector() in UserManager is not virtual anymore. Also removed other implementations.
  - This had to be done as get_all_as_vector() cannot be customized by
    scripts, and if an inherited UserManager needs to set up UserModules
    this will cause issues later down the line.
  - Other solutions like a user_setup() method could also work, but then
    that would cause more individual (non-optimizable) lookups to db-based
    backends (and likely others too), so this solution seems to be the best
    currently.
- Improved UserManagerFile's internals.
- Now UserManager::_create_user() has a User as a parameter.
  This shoukld make it easier to customize user creation. Similar idea to
  how TerrainWorld's _create_chunk() works.
- Fix calling the new _create_user() method in UserManager.
- Fix saving files in UserManagerFile.
- Fix User::_from_dict() using wrong dictionary keys.
- Disable internal process for UserManagerFile. It's not needed anymore.
- Guard against bad indexes in User::_from_dict().
- Write and read lock modules in User::_to_dict() and User::_from_dict().
- Use module names instead of indexes if they are available in User::_from_dict() and User::_to_dict().
- Added some comments to User.
- Added read_try_lock and write_try_lock to User and UserModule.
- Docs for the User class.
- Removed unused enum from User. Note that WebPermission had the same enum if needed.
- Always just defer call to the active UserManager or error in UserDB.
  - Having an optional separate User list in UserDB when there is no
    UserManager can lead to subtle bugs, so it got removed.
  - It also had other limitations, like it was not possible to setup Users
    properly, which could also lead to subtle bugs.
- Don't use the changed signal to get a User to save itself.
- Now UserManagers have a register_as_global property. A manager will only try to set itself as the global manager if it's set to true (default).
- Print an error if more than one UserManagers are trying to become the global instance.
- Docs for UserManager.
- Store the owner UserManager in User.
- Store the owner UserManager in User as an ObjectID.
- Use the owner UserManager when saving Users.
- Added internal user name and email support for Users.
- Updated the UserManagers to use the new internal name and email. Also made sure to call the base _create_user in all UserManagers.
  - Internal name and email is used for lookups, so capitalization automatically won't matter.
- Set maximum password length in UserRegisterWebPage just for good measure (256).
- Also set a maximum password length of 256 in UserLoginWebPage.
- Remove unneeded comments in UserLoginWebPage.
- Set maximum password length of 256 in UserSettingsWebPage aswell.
- Remove unneeded comments in UserSettingsWebPage.
- Include cleanups in UserWebPages.
- Added missing user locks.
- Fix missing tag in UserPasswordResetWebPage's docs.
- UserManagerDB now can update it's tables to the newer format.
- Fix UserManager::_notification() switch fallthroughs.
- Fix User::_from_dict() not setting internal name and email.
- Full and improved docs for the entire module.

#### tiled_wall

- Return the actual AABB in TiledWall::get_aabb().
- Added an editor gizmo plugin for TiledWalls so they can be selected in the spatial editor directly.

#### props

- Now PropInstance inherits from VisualInstance. Set up it's AABB calculation.
- Added a simple gizmo for PropInstances. This makes PropInstances selectable with the mouse in the Spatial editor.
- Don't use octahedral compression in the prop module's gizmos.

#### terraman

- Added TerrainWorldChunkDataManager class to support loading and saving TerrainWorld chunks dynamically to and from disk.
- Now TerrainWorld uses TerrainWorldChunkDataManagers if they are available.
- Now TerrainWorld won't save it's chunks into scenes in the editor anymore if a TerrainWorldChunkDataManager is set.
- Added chunk saving api to TerrainWorldChunkDataManager.
- Added force_save_all_chunks() method to TerrainWorld.
- Added a force save all chunks button to TerrainWorld's inspector if a TerrainWorldChunkDataManager is set.
- Implement TerrainWorldChunkDataManagerStaticFolderResources.
- Split _create_chunk() to _create_chunk() and _setup_chunk() in TerrainWorld.
  Unfortunately this is a breaking change. (Very slight though.)
  Now _create_chunk() should be used to initialize serialized properties
  in chunks. The new _setup_chunk() virtual should be used to initialize
  other non/serialized properties (like meshers).
  Fortunately the only thing that needs to be done is to split old
  _create_chunk() into two.
  Note that when using procedural generation, the old way should just work
  without any updates necessary. This change is only needed when loading
  of chunks is desired.
- Fix error message on startup.
- Call TerrainWorldDefault's _create_chunk() and _setup_chunk() in TerrainWorldBlocky.
- Call chunk's enter_tree and set voxel world on entering the tree in TerrainWorld.
- Only build the chunk in TerrainWorld::chunk_add() if the world is in the tree.
- Fix visibility toggling in TerrainChunkDefault even if lods are disabled.
- Also check whether chunks are building when doing a priority generation.
- Bind is_priority_generation bool in TerrainWorld. Also set it to false in the constructor, but set it to true when eneteeing tree if the world already has chunks loaded.
- Don't destroy and re-generate meshes in TerrainChunk's _enter and _exit_tree, just hide / show them. This makes scene tab swithing with TerrainWorlds in the editor a lot simpler.
- TerrainChunkDefault lod change code cleanups and improvements. Also set scenario on entering and exiting the tree.

#### entity_spell_system

- Re/enabled the entity_spell_system's editor plugin, and disabled it's tool menu item.
- Added 3 new helper classes for the entity spell system. ESSEntityWorldSpawner3D, ESSEntityWorldSpawner3DSingle, ESSEntityWorldSpawner2D.
- Added a Spatial Gizmo plugin for ESSEntityWorldSpawner3D.
- Write the spawner's name to the world in the WorldSpawner3DSpatialGizmoPlugin.
- Implemented ESSEntityWorldSpawner3DSingle.
- Rotate the text by 180 degrees in WorldSpawner3DSpatialGizmoPlugin.
- Call spawn as deferred in ESSEntityWorldSpawner3DSingle instead of requesting a deferred entity spawn.
- Now Entities can store their spawner's ObjectID.
- Implemented respawn support for ESSEntityWorldSpawner3DSingle.
- Added ESSEntityWorldSpawner3DArea class.
- Added spawn_area_extents property to ESSEntityWorldSpawner3DArea and implemented an editor gizmo for it.
- Renamed the level property to entity_level in ESSEntityWorldSpawner3DSingle.
- Zero out the entity's ObjectID in ESSEntityWorldSpawner3DSingle when exiting tree.
- Fix the type of the respawn time properties in ESSEntityWorldSpawner3DSingle.
- ESSEntityWorldSpawner3DArea initial implementation.
- Implemented spawn entry editing to ESSEntityWorldSpawner3DArea.
- Fix drawing the spawn slots in ESSEntityWorldSpawner3DArea's gizmo.
- Fix the spawn position line height in WorldSpawner3DSpatialGizmoPlugin.
- Fix drawing line colors in WorldSpawner3DSpatialGizmoPlugin. Also better colors.
- Added prop support for ESSEntityWorldSpawner3DSingle.
- Added prop support for PropDataESSEntityWorldSpawner3DArea.
- Added PropDataESSEntityWorldSpawner3D class. Made PropDataESSEntityWorldSpawner3DArea and PropDataESSEntityWorldSpawner3DSingle inherit from it.
- Fix the type of the auto_learn_spells property in the ESS singleton.
- Fix remaining_absorb's setter in AuraData.
- Fix _gets_relation_to() and _getc_relation_to() virtual method bind in Entity.
- Fix virtual method bind in CharacterSkeleton2D and CharacterSkeleton3D.
- Fix signal name bind in ESSEntityWorldSpawner2D and ESSEntityWorldSpawner3D.
- Fix name for the charges_changed signal in ItemInstance.
- Zero out target_stat_id in StatData.
- Full and improved docs fot the entire module.

#### totp

- Created a new TOTP module. TOTP stands for Timed One Time Password.

#### fastnoise

- Full and improved docs for the entire module.

#### lz4

- Full and improved docs for the entire module.

### Other

- Updated build containers from upstream godot.
- Remove community stuff from the readme, as it's not really relevant. Also mention a solution in case this becomes an issue (but it likely won't be).
- Removed TODO.md. Not needed anymore.
- Removed notable_godot_commits_not_included.md. Not needed.

### Backports

- Backported everything up to and including https://github.com/godotengine/godot/commit/e430053b16fd6edd8ddc00801e8ea4981bff12de
  merge commit: https://github.com/godotengine/godot/commit/84f761ba54d540f17c7623506318334ce602eb68

#### Godot 3.x

- Backported parts of be6f971f4ff from godot 3.x.
  Use the in-built casting instead of dynamic_cast on all platforms
  The in-built casting appears significantly faster than `dynamic_cast`.
  - lawnjelly, Ivorforce
  https://github.com/godotengine/godot/commit/be6f971f4ff7b3647ac576ddc5813d462fe6fec9
- Physics Interpolation - Fix `CPUParticles` to work with `SceneTreeFTI`
- Physics Interpolation - Reduce unnecessary `VisualServer` updates
  With the new `SceneTreeFTI`, most xforms are updated to the server externally by the FTI system, so it is no longer necessary to update the server on each `NOTIFICATION_TRANSFORM_CHANGED`.
- Physics Interpolation - Add InterpolatedProperty
  And add some basic interpolated properties to Camera.
- Physics Interpolation - Fix non-interpolated resting xforms
  Ensure servers are updated for non-interpolated Spatials, either during the scene tree update or a final pass.
  Ensure properties and xforms are given a final server update in the final resting positions after removal from tick lists.
  Fixes dirty local xform bug.
- Physics Interpolation - Fix `disable_scale` bug in 3D
- FTI - Fix 3D auto-resets
  * Ensure NOTIFICATION_RESET_PHYSICS_INTERPOLATION is sent to derived classes
  * Add deferred auto-resets for all `Spatials` on entering the tree
- Add an editor option to copy system info to clipboard
- FTI - Add custom interpolation for wheels
- FTI - Fix `SceneTreeFTI` behaviour on exit tree
- Make selected tile in TileSet more visible through red outline
- Track external changes in the custom fonts set by BBCode / `push_font`.
- Improve set_radial_initial_angle by removing loops
- Fix mouse_over not dropped when mouse leaves window
- Cancel tooltips when mouse leaves window
  This is a backport of 807431c49a6b33ecc88f8d4ebcb3b2f359591b1c.
- Improve Class display in Create dialog
- Fix double TOOLS_ENABLED checks from NO_EDITOR_SPLASH.
- Expose some helper methods on Viewport
  Expose gui_release_focus and gui_get_focus_owner to Viewport
  Expose a method to get hovered Control in Viewport
  Co-Authored-By: Gilles Roudiere <gilles.roudiere@gmail.com>
  Co-Authored-By: Claire Blackshaw <evilkimau@gmail.com>
- FTI - Optimize `SceneTree` traversal
- Fix unzSeekCurrentFile not resetting total_out_64.
- `SceneTreeFTI` - fix identity_xform flag getting out of sync
  This could cause incorrect rendered xform for one frame.
- Show TextureProgress radial cross only when editing the scene
- `SceneTreeFTI` - Fix `force_update` flag for invisible nodes
- FTI - `global_transform_interpolated()` on demand for invisible nodes
- Pre-calculate `is_visible_in_tree()`
- FTI - Reduce `VisualInstance` xform notifications
- certs: Sync with upstream as of Apr 8 2025 (cherry picked from commit f5eaf2a57687ee45a9484b53c9397b071b28ffbf)
- mbedTLS: Update to version 2.28.10 (cherry picked from commit cdb875257a08bc3ae632b78c050293276aefe905)
- Initialize pa_buffer_attr.maxlength to -1 (cherry picked from commit b5622e9f780503a4b24300541bac070772721f5e)
- Disable Nahimic code injection. (cherry picked from commit 8bb3e5360ed21ae89d53cdc3fb1c0dbc756262bb)
- Fix Xbox Controller on Android (cherry picked from commit cf00265386b98da3f24f5c3de22358fbf2e1a46b)
- Remove implicit conversion from `LocalVector` to `Vector`
- Remove implicit conversion from `LocalVector` to `PoolVector`
- `Spatial` and `CanvasItem` children change to `LocalVector`
- Allow constructing Quat from two Vector3s
- Move set_shortest_arc to it's proper place.
- ResourceImporterWAV: Detect if data chunk size is larger than the actual size
- Provide quick access to `Object` ancestry
- Remove `_is_vi_visible()`. Can be replaced by `is_visible_in_tree()`.


## [4.6.0]

Highlights:

- Terraman got a new fully featured editor. It also got heavy improvements, including being able to work with the vertex lights 3d system.
- Added easier locale support for the web module.
- Usage improvements to the database module.
- PLogger is now a separate app-side logging layer.
- If a .csv file is dropped into a project the editor will not import it by default.

Breaking changes:

- Mass replaced the typo "tarnsform" to "transform".
- Removed set_voxel_with_tool from TerrainWorld.

### Core

- Moved the static_assert in GLOBAL_CACHED to the top.
- Reworked the new GLOBAL_GET_CACHED macro.
- Use HAS_TRIVIAL_DESTRUCTOR macro instead of std::is_trivially_destructible.

#### PLogger

- Added log_custom method to PLogger. It's a lot more customizable than the others by design. Also small cleanups.
- Added a new important log level to PLogger.
- Added force printing / logging option to OS. Made PLogger use this internally.

### Math

- Added set_look_at helper method to Basis.
- Transform now uses Basis::create_looking_at.
- Added non-static looking_at and from_scale helper methods to Basis.
- Undo unecessary breaking change to gdnative.
- Renamed looking_at to create_looking_at, and from_scale to create_from_scale in Basis.

### Scene

- Added NOTIFICATION_QUITTING to MainLoop.
- Inlined set_pressed_no_signal call into BaseButton::set_pressed().
- Hide the Bake text of the MergeGroupEditorPlugin's icon.
- Set use_on_initial_import() in ResourceImporterCSVTranslation to false.
  This means that if a .csv file is dropped into a project the editor will
  not import it by default.
- Added use_on_initial_import() virtual method to ResourceImporter.
  If it returns false and the resource importer would be used on a file's
  initial import it will be imported with the "Keep File (No Import)"
  setting.

### Modules

#### Terraman

- Now TerrainChunks will register their lights into the VertexLights3DServer if use_vertex_lights_3d is enabled in world.
- Make use of the VertexLights3DServer in TerrainChunkDefault::_bake_lights() if enabled.
- Added use_vertex_lights_3d property to TerrainWorld.
- Added VertexLight3D baking tool to TerrainWorldEditor.
- Fix TerrainChunk::light_remove_index() not actually remoing lights.
- Added more helper methods to TerrainWorld.
- Added item_cull_mask property to TerrainLight.
- Added light_mode property to TerrainLight.
- Removed OWNER_TYPE_VERTEX_LIGHT_3D from TerrainLight.
- Small tweaks to the info outputs in TerrainWorldEditor.
- Fix method call in TerrainWorldEditor.
- Added MeshDataInstance baking tool to TerrainWorldEditor.
- Added mesh_data_resource_add() helper method to TerrainWorld.
- Now TerrainChunks can also store a name and material for MeshDataResources.
- Don't create prop baking tool buttons if the Props module is disabled.
- Added a scene baking tool to TerrainWorldEditor.
- Fix logic in TerrainChunk::scenes_set() and TerrainChunk::props_set().
- Return and set the name with the props property in TerrainChunk.
- Implement setting a name to scenes in TerrainChunk.
- Added tooltips for the bake props tool's buttons in TerrainWorldEditor.
- Now props added to TerrainWorld can also have a name. The prop bake editor tool uses it to save and restore node names.
- Implemented Bake Props tool for TerrainWorldEditor.
- Rebuild chunks in TerrainWorld::prop_add().
- Make sure the prop meshes are cleared from previous runs before returning early in TerrainPropJob.
- Call emit_changed() in TerrainChunk setters. Also a setter fix.
- Set up baking tools page in TerrainWorldEditor.
- Fix potential hang on exit due to deadlocks.
- More tweaks to TerrainChunk's new scene api.
- Added the new scene_add() method to TerrainWorld. Also store scenes from props inside chunks.
- Tweaks and improvements to TerrainChunk's new scene api.
- Added scene storage api to TerrainChunks.
  Also mass replaced the typo "tarnsform" to "transform", and fixed a
  parameter name in a TerrainChunk binding.
- Added owned_lights property to TerrainChunk. Also changed the lights property it is now scripting only, it won't get saved.
- Now TerrainLights inherit from Resource.
- Added to_dict() and from_dict() methods to TerrainLight.
- Set prop ownership to lights in TerrainWorld::prop_add().
- Store ownership information in TerrainLights.
- Store whether a mesh_data_resource in a TerrainChunk is original or not.
- Renamed the new original parameter in TerrainChunk and TerrainWorld's prop api from owner to original.
- Set prop ownership when adding them in TerrainWorld.
- Now TerrainChunks will remember whether they own a prop or not. Also added and bound missing methods to Chunk's prop api.
- Make sure the remaining chunks are notified of the changed world lights when adding / removing chunks.
- Now TerrainLights are stored inside chunks (they are also automatically saved with them). Also changed TerrainLight's api to makes use of Vector3i.
- Now props and mesh data resources are saved with the TerrainChunks.
- Only generate chunks once.
- Simplify chunks_set() in TerrainWorld.
- Fix isolevel paint and paint brush tools always allowing chunk creation.
- Add the new chunk to the generation queue instead of directly calling it's build method in TerrainWorld::chunk_add().
- Also expose the liquid data in TerrainChunkBlocky.
- Fix typo.
- Implemented the spawn chunks and remove chunks tool in TerrainWorldEditor.
- Call TerrainChunk's build() after it's added to a World manually.
- Added a new helper method to TerrainWorld.
- Clear the material cache data in TerrainChunk::_exit_tree().
- Implemented the gui for the chunk spawn and chunk remove tools in TerrainWorldEditor.
- TerrainWorldEditor variable name tweaks.
- Implemented the paint picker tool in TerrainWorldEditor. Also improved the editor's Button signal setup.
- Tewaks to the main tool labels in TerrainWorldEditor.
- Implemented the paint picker tool's ui in TerrainWorldEditor.
- Add the paint brush tool settings buttons to the proper container in TerrainWorldEditor.
- Reordered the main tool buttons in TerrainWorldEditor.
- Removed the add and remove single tools from TerrainWorldEditor as they are not very useful for a terrain engine.
- Only show the surface selection when the paint brush is active in TerrainWorldEditor.
- Added the ability to erase using the paint brush in TerrainWorldEditor.
- Added liquid mode to the isolevel brush and paint brush tools in TerrainWorldEditor.
- Added a new helper method to TerrainWorld.
- Chunk remover tool initial setup in TerrainWorldEditor.
- Make the expected tool settings shown by default in TerrainWorldEditor.
- Spawn brush initial setup in TerrainWorldEditor.
- Reorganized the tools in TerrainWorldEditor. Also made the Isolevel Brush the default.
- Renamed the add and remove tools to add single and remove single voxel in TerrainWorldEditor. Also improved their gui a bit.
- Disable allow chunk creation for the paint brush and isolevel brush tools by default in TerrainWorldEditor. A new chunk spawner tool will be added instead soon.
- Invert the logic of the isolevel brush smoothness calculation so it works as expected.
- Start at max isolevel brush smoothness in TerrainWorldEditor.
- Fix isolevel brush smoothness calculation in TerrainWorldEditor.
- Allow 0 as the isolevel brush strength and smoothness in TerrainWorldEditor.
- Handle a missing case when setting voxels in TerrainWorld.
- Reset the mesher's lod index in _reset() in TerrainTerrainJob. This fixes subsequent mesh generations.
- Implemented chunk material invalidation support for TerrainLibraryMergerPCM.
- Unlock the mutex after the rects are refreshed in the material cache in TerrainLibraryMergerPCM.
- Optimized editing in TerrainWorldEditor.
- Use the new immediate build mode in TerrainWorld's set_voxel_at_world_data_position().
- Implemented an immediate build mode for TerrainChunk.
- Clear meshes as needed in TerrainTerrainJob.
- Clear meshes as needed in TerrainPropJob.
- Fix crash on shutdown.
- Udpate the gizmo size properly in TerrainWorldEditor.
- Optimized UndoRedo usage in TerrainWorldEditor.
- Implemented the TerrainWorldGizmo.
- Renamed the new gizmo.
- Initial gizmo setup for Terraman.
- Moved the TerrainWorldEditor to a new folder. Also make sure it's only compiled in tools builds.
- Implemented the paint brush mode in TerrainWorldEditor.
- Implemented the isolevel brush in TerrainWorldEditor.
- Added new helper methods to TerrainWorld.
- Added a strength slider for the isolevel brush in TerrainWorldEditor.
- Added a button for setting the allow chunk creation option for the isolevel brush in TerrainWorldEditor.
- Removed set_voxel_with_tool from TerrainWorld.
- Initial input handling refactor for TerrainWorldEditor.
- Added the option to select the channel in TerrainWorldEditor's Isolevel Brush.
- Gui and callback setup for the isolevel brush in TerrainWorldEditor.
- Small tweaks and reorganization in TerrainWorldEditor.
- TerrainWorldEditor cleanups and some initial rework for it's new functionality.
- Tidy up the Isolevel clider of the TerrainWorldEditor.
- Moved the control buttons of the TerrainWorldEditor to the side panel.
- Fix variable names.
- Fix property names.
- Use icons for the TerrainWorldEditor's buttons.

#### Web

- Added new classes to the web module's config.py.
- Added a new LocaleSetupWebServerMiddleware.
- HTMLTemplateMultilang now also tries to use the locale helper method in WebServerRequest.
- Implement locale helper methods to WebServerRequest.
- Bind methods in HTMLTemplateMultilang.
- Created a HTMLTemplateMultilang class. It can automatically select between multiple HTMLTemplates.

#### Users

-Added a method to get all users from the UserDB and UserManagers.

#### Database

- Added next_column*() helper methods to PreparedStatement.
- Now get_cell and is_cell_null in Sqlite3QueryResult will do a bounds check via error macros.
  This means indexing errors will not result in a crash due to vectors using CRASH_BAD_INDEX error macros.
- Added get_stored_row_count() and get_cell_count() helper methods to QueryResult.
- Added get_next_cell*() helper methods to QueryBuilder.
- Added get_last_insert_rowid() method to PreparedStatement.

#### Database - Sqlite

- Reapply SQLITE_NO_FCHOWN Pandemonium sqlite patch. also added it as a .patch file.
- Updated sqlite to version 3.47.0. Also added a VERSION.txt to it's folder.

### GDnative

- Update gdnative api.

### Backports

- Backported everything up to and including https://github.com/godotengine/godot/commit/31935d6d636ed463f607b3bfeadb604404ec53ca merge commit: https://github.com/godotengine/godot/commit/157f8805c215adc84ac26e14009e02b916336699

#### Godot 3.x

- Backported move semantics from godot 3.x. with some changes. Original commit: https://github.com/godotengine/godot/commit/d549b98c5cb4b68f38aece3171cd0aac7c63a7a7
- Physics Interpolation - Move 3D FTI to `SceneTree`
  Moves 3D interpolation from `VisualServer` to the client code (`SceneTree`).
  Complete rework of 3D physics interpolation, but using the same user API.
- Simplified `ObjectDB::get_instance()` casting
  Reduces boiler plate by templating `get_instance()` for the cast type, while remaining backward compatible to the existing functionality.
- Make drop_mouse_on_gui_input_disabled GLOBAL_GET cached.
- Drop mouse focus and over when gui input is globally disabled
  Since some porjects may be relying on the former behavior, this is opt-in via a new project setting, disabled by default, but enabled for new projects, since it's the new standard behavior (and the only one in 4.0).
- Add GLOBAL_GET cached macros.
  GLOBAL_GET is an expensive operation which should not be used each frame / tick.
  This PR adds macros which do a cheaper revision check, and only call the expensive GLOBAL_GET when project settings have changed.
- Prevent inlining error print functions.
- Docs: BitMap: Added description for opaque_to_polygons method
- Physics Interpolation - fix client interpolation pump
  Client interpolation pump is moved AFTER the physics tick, after physics objects have been moved.
  This is necessary because the `current` transform is also updated during the pump.
- Physics Interpolation - Auto-reset on `set_physics_interpolation_mode()`
  Fixes historical bug where auto-reset wasn't working correctly.
  Also fixes process modes on Cameras when mode is changed.
- Revert changes to Range::set_value #65101
- Fix VS project generation with SCons 4.8.0+
- mbedtls: Update to upstream version 2.28.9
- embree: Fix invalid output operators raising errors with GCC 15
- Enable builds with miniupnpc API 18
  Backports https://github.com/godotengine/godot/pull/97139 without
  bumping the embedded miniupnpc library.
- Document radial_center_offset bounds
- Ameliorate performance regression due to directional shadow `fade_start`
- Hide last DirectionalLight shadow split distance property when using PSSM 3 Splits
  The last shadow split property only has an effect when using PSSM 4 Splits.
- Improve cache handling
- Add `EditorPlugin::scene_saved` signal (3.x backport)
  Matches the `EditorNode` one for parity with the exposed
  `resource_saved` signal
- Document Timer autostart in tool scripts
- Don't cache emsdk
  Due to how caches are accessed this cache is almost useless, it only
  matters if it is from the same branch or a base branch, and is identical
  between branches, so caching it just clutters the build cache
- Implement glow map effect
- Fix split_floats behavior when spaces are used as separators
- CI: Remove now unused sources.list file
- CI: Update Ubuntu runners to 24.04, but keep 22.04 for Linux builds
  Pin clang-format to version 16, and black to 24.10.0.
  Keep using Ubuntu 22.04 for Linux builds for portability.
- certs: Sync with Mozilla bundle as of Oct 19, 2024
  https://github.com/bagder/ca-bundle/commit/4d3fe6683f651d96be1bbef316b201e9b33b274d
  Document matching mozilla-release changeset.
- Fix error when non-ASCII characters in resource pack path
- Fix `PopupMenu` margin and separation calculations
- Fix random multithreaded crash that happens when setting the audio stream on a AudioStreamRandomPitch stream.
- Fix Button not listing `hover_pressed` stylebox
- Fix button click detection when `Tree` is rotated
- Cache results for `TranslationServer.compare_locales()`
- Backport 65910 for Godot 3
- Do not auto add default script and country codes to the locale.
- Document that `Input.is_action` should not be used during input-handling
  In most cases `InputEvent.is_action*` is more appropriate during input-handling.
- Fix physics platform behaviour regression
  Lifetime checks for stored `RIDs` for collision objects assumed they had valid `object_ids`.
  It turns out that some are not derived from `Object` and thus checking `ObjectDB` returns false for some valid `RIDs`.
  To account for this we only perform lifetime checks on valid `object_ids`.
- Improve error message when `OS.execute()` fails on Windows
  This takes the error message from 4.x and backports it to 3.x.


#### Godot 4


- Make SkeletonIk3D node usable - warriormaster12 https://github.com/godotengine/godot/commit/ddf93f3f59aa5b31908792821e3dfa11ab7b8673
- Add the ability to look-at in model-space.
  This is a much simpler attempt to solve the same problem as #76060, but
  without breaking any compatibility.
  * Adds a description of what model space is in the Vector3 enums
    (MODEL_* constants). This has the proper axes laid out for imported 3D
    assets.
  * Adds the option to `look_at` using model_space, which uses
    Vector3.MODEL_FRONT as forward vector.
  The attempt of this PR is to still break the assumption that there is a
  single direction of forward (which is not the case in Godot)
  and make it easier to understand where 3D models are facing, as well as
  orienting them via look_at.
  - reduz 
  https://github.com/godotengine/godot/commit/5fdc1232eff45e31ee53f58e618de6c58d3f7203

### Docs

- Update the readme.
- Updated the links in CONTRIBUTING.md, also small tweaks.
- License tweaks, also fixed project information in COPYRIGHT.txt.
- Experiment with a different changelog style.


## [4.5.0]

Highlights:

- New Procedural Tree 3D Module. Adds a new 'ProceduralTreeMesh' Mesh class that can generate trees procedurally. It uses https://github.com/jarikomppa/proctree .
- Backported text-to-speech support.
- Now the releases include a gdnative headers zip.

Breaking changes:

- None.

### Added

#### Engine

- Backport text-to-speech support.

#### Modules

##### Procedural Tree 3D

- New Procedural Tree 3D Module. Adds a new 'ProceduralTreeMesh' Mesh class that can generate trees procedurally. It uses https://github.com/jarikomppa/proctree .

#### Release

- Now the releases include a gdnative headers zip.
- Reworked the copy_repos script to use git locally to make a copy from the local engine tree.
- Copy gdnative headers helper script.

### Fixed

- Make sure the thumbnail view is the default in the FileSystemDock as intended.
- Only keep one of the doc links in the editor's help menu.
- Updated the engine license in the EditorAbout window.
- Merge pull request from halotroop2288: Separate statements about Godot and Pandemonium in README

### Removed

- Removed feature profile dir creation from the editor.
- Removed the support development option from the help menu.
- Removed the donors tab from the EditorAbout window, as I'm not even trying to take donations anymore.

### Backports

- Backported everything up to and including https://github.com/godotengine/godot/commit/0ccd559d176b5e0d0c72128c382b6cad525438c2 Merge commit: https://github.com/godotengine/godot/commit/8c444fb9c9ed70306630ebaf4868a6629df1296c

#### Godot 3.x

- `Object::call()` prevent debug lock accessing dangling pointer
  Self deleting an object within a call was leading to crashes due to referencing freed memory, due to a raw pointer stored in the debug lock.
- Fix parsing of `4.` in Expression
- Backported: Doctool: Remove version attribute from XML header. We don't use that info for anything, 
  and it generates unnecessary diffs every time we bump the minor version (and CI failures if we forget 
  to sync some files from opt-in modules (mono, text_server_fb).
- Safe `Camera::unproject_position()`
  `unproject_position()` can fail in some circumstances, and this needs to be conveyed to calling code.
- PopupMenu: Update margins on visibility change Fixes #96149.
- Verify GLTF indices to prevent crash with corrupt files
  Also verify prior to vertex optimization.
- GLTF: Fixed external images getting embedded on import
  Added a map to keep track of external images during import, and used
  that map to instance the textures using the resource loader instead of
  creating a new texture from scratch

## [4.4.0]

Highlights:

- Tweaks to editor defaults.
- Added back lightmaps, gpu based particles (GPUParticle) and GIProbes.
- Added back the gles3 renderer (can be turned off compile time).
- LayeredTileMaps got VertexLight2D management, and navigation obstacle support.
- Implemented prepared statements for the database module.
- The markdown renderer is now scriptable.

Breaking changes:

- Always defer unmatched requests in WebNodes to index nodes if they are available.
- Tweak WebNode::get_full_uri() to give back the output what you'd normally assume when the uri segment is "/" under a WebRoot.
- Renamed the render_menu property in UserWebPage to should_render_menu, as there is a render_menu method in WebNode.
- Don't create any accounts by default in the UserManagerDB::_create_default_entries() virtual method.
- Hide result String in TableBuilder. Also add reset() method to it. (c++ only)
- Made query_result in QueryBuilder protected. (c++ only)

### Added

#### Core

- Added more helper methods to StringBuilder.

#### Scene

- Added a group for the toplevel property in Node2D.
- Moved the y_sort property from YSort to Node2D. Node2d has it off by default while YSort has it on by default.

#### Rendering

- Added back the GLES3 renderer. Unlike in godot it can be disabled compile time.
- Added back GIProbes.
- Added back the gpu based particles. Renamed Particle to GPUParticle and Particle2D to GPUParticle2D.
- Added back lightmaps.

#### Modules

- Added back the denoise module, with oidn built in.
- Added back the lightmapper_cpu module.
- Added back the raycast module with embree built in.
- Added back the scene side lighmapper classes.
- Added back the xatlas_unwrap module. Also moved xatlas into it so it's not in the thirdparty folder.

##### Database

- Finish missing docs for the rest of the database module.
- Added more helper methods to deal with prepared statements to QueryBuilder.
- Renamed clear to reset in TableBuilder.
- Added virtual methods to QueryBuilder for adding prepared statement placeholders.
- Setup prepared statement support for the database module.

##### Database SQLite

- Prepared statements for the sqlite database backend.

##### LayeredTileMaps

- Implemented VertexLight2D management support to LayeredTileMapLayers.
- VertexLight2D support for LayeredTileSetAtlasSourceEditor.
- Added VertexLight2D support to LayeredTileData.
- Implement debug drawing avoidance radius in LayeredTileMapLayer.
- Implement navigation obstacle support for LayeredTileMapLayers.
- Added avoidance position property to LayeredTileData.
- Added flip winding order tool to GenericTilePolygonEditor.
- Implemented Obstacle for LayeredTileSets.

##### Web

- Docs for the markdown renderer.
- Added the ability to script the rendering of the MarkdownRenderer. Also added a new render() method equivalent to render_to_html().

### Fixed

#### Core

- Don't access a Message's memory after calling it's destructor in MessageQueue. (Even though the way the code works currently this was not really a bug, but it can easily become one.)
- Fix size mismatch warning.
- Remove unnecessary template parameters from constructors and desctuctors.

#### Scene

- Fix the type of the rotation tracks in AnimationTrackKeyEdit.

#### Servers

- Don't error in RenderingServerRaster::free() if the supplied RID is just invalid.

#### Editor

- Improvements to the filesystem dock.
- Improved the renderer selector when creating a new project.

#### Modules

##### Database

- Fix text binds being interpreted as blobs.

##### GDScript

- Update theme item names for pandemonium.

##### LayeredTileMaps

- Fixed rao and vertex lights for newly added cells. Also fixed re-generating the tilemap cells in many cases. Also small improvements.
- Remove duplicate call.
- Bind _tile_set_changed in LayeredTileDataEditor.

##### Web

- Set the status code in WebRoot::_handle_error_send_request().
- Optimize _node_route_map's usage in WebNode.
- Fixed docs, index node support has been moved from WebRoots to WebNodes a while ago.
- Use HashMap in BrowsableFolderServeWebPage, as the bug that was preventing this was already fixed quite a while ago.
- Don't expose renderer_callback() for scripting.

#### Utilities

- Updated the clang format script.

### Changed

#### Editor

- Tweaks to the editor defaults.

#### Modules

##### Database

- Use StringBuilder in TableBuilder aswell.
- QueryBuilder now uses a StringBuilder internally.
- Hide result String in TableBuilder. Also add reset() method to it.
- Made query_result in QueryBuilder protected.
- DatabaseConnection::database_connect() now returns Error.

##### Database SQLite

- Fix getting utf-8 texts from Sqlite3QueryResult.

##### GDScript

- Disabled the unused argument warning by default.

##### LayeredTileMaps

- Turn autowrap on on LayeredTileSetAtlasSourceEditor's tile_inspector_no_tile_selected_label.

##### Users

- Don't create any accounts in UserManagerDB::_create_default_entries().

##### Web

- Always defer unmatched requests to index nodes if they are available.
- Expose FileCache in FolderServeWebPage to scripts.
- Tweak WebNode::get_full_uri() to give back the output what you'd normally assume when the uri segment is "/" under a WebRoot.
- Renamed the render_menu property in UserWebPage to should_render_menu, as there is a render_menu method in WebNode.

### Backports

- Backported everything up to and including https://github.com/godotengine/godot/commit/1cf50364c116acab4f86b70c2fd559475d60e198 Merge commit: https://github.com/godotengine/godot/commit/2cc5ca43ef68382c94d963bd9336ad8efea6adae

#### Godot3

- Backported: [CI] Upload build cache before running tests. - AThousandShips https://github.com/godotengine/godot/commit/078210bce16b9d7e4109db938560ad17c28a10c7
- Fix `PopupMenu` size calculations not taking into account control/canvas scale
- fix lost old callback when continuous call requestRenderAndNotify
- Backport DirectionalLight `fade_start` property to `3.x`:
  Implement shadow fading when using the Orthogonal shadow mode (like in `master`).
  This allows customizing the distance at which directional shadows start to fade away. Shadow fading will also always start at the same distance now, regardless of the current shadow mode in use.
  This is useful for enclosed levels to prevent shadows from fading at all with a well-tuned maximum distance.
  The default fade start value (0.8) results in fading happening later in the distance compared to the previous behavior, where fading started from the last shadow split distance (0.6 in PSSM 4 Splits and 0.1 in PSSM 2 Splits).
- Fix physics tick count in `Input.action_press` and `Input.action_release` The physics tick count was not yet updated there.
- Use unztell64 in FileAccessZIP to ensure 64 bit return (cherry picked from commit efccebd3db90d7baf0947cd02612ab987ab87868)
- doc: Fix cherry-pick mistake for Vector3.cross description
- Add POST_NOTIFICATIONS permission to the list of permissions available in the Export dialog (cherry picked from commit 739190ca2b7f1b67a78eff33154a4bfd1964f5c4)
- Clarify the behavior of `Vector2/3.cross` and mention parallel vectors (cherry picked from commit 47a8033698b14c8a7bb25867198c1371382e3398)
- Always look for unique node names in owner if not found in owned nodes (cherry picked from commit 95ced4bbdcea4d8e225e235fc120c4ebd72f443d)
- Enabled secure restorable state. (cherry picked from commit 84380a94f776c8c1e7786d2eb3fb811789f42639)
- miniupnpc: Disable socket timeout on Windows, matching upstream Fixes #88471. (cherry picked from commit f695de7c68e373088175f8f3b1650f11a27be7aa)
- Add support for PCK embedding section with non GNU-ld linkers. (cherry picked from commit 625c4bdacef4173f94200632362e25e537ad5d21)
- Physics Interpolation - refactor client interpolation pump
  * Move client interpolation pump to earlier in the iteration before 3D physics synced
  * Allow `get_global_transform_interpolated()` to prime the client interpolation inside a physics tick
- Physics Interpolation - refactor `Camera` and fix `get_camera_transform()`
  * Moves 3D Camera interpolation scene side.
  * Automatically switches `get_camera_transform()` to report interpolated transform during `_process()`.
  * Fixes `ClippedCamera` to work with physics interpolation.
- Physics Interpolation - Fix 2D skinning
  2D skinning required the interpolated skeleton base transform to be updated when using interpolation.
- Fix `spatial_editor_plugin` enum
  * The `view_mesh_stats` feature introduced a new enum value `VIEW_SLECTED_INFO` within the unnamed `VIEW` enum.
  * This turns out to be saved somehow, which means the view settings can get out of sync when reloading the project in an older version of the editor.
  * The solution is simply to move the new value to the end of the enum so there are no conflicts.
- Editor 3D view mesh stats
  Similar to information window, add a small optional window to display face count and other stats.
- Add IME input support.
- Add support for privacy manifest configuration.
- Extend iOS plugins to support Swift runtime
- Update actions.
- Web: Bump closure compiler spec to ECMASCRIPT_2021 Fixes #88008. (cherry picked from commit d29b0d90e0d735eee860b977c57cb75423d7a9ba)
- Fix emscripten 3.1.51 breaking change about `*glGetProcAddress()` (cherry picked from commit 5922ac0fb11105da67d28847b60a521406a77cdb)
- Add `WASM_BIGINT` linker flag to the web build (cherry picked from commit 3ae524fa9e42828183f8046f6f33b945475bb052)
- Fix gradle build errors when the build path contains non-ASCII characters (cherry picked from commit f1887a30f35dafac405014ef5ca6e3d76b4da960)
- Add basic Emacs .gitignore entries (cherry picked from commit 2696fee3c6b08cd9645a61ebf08b84fd70be5a72)
- Sync controller mappings DB with SDL 2 community repo Synced with mdqinc/SDL_GameControllerDB@5b4efa3a2015552cec4cb809169d29615202e9f2 (cherry picked from commit 1da02fa7be4ace876a53802d19711ec7037d94f3)
- Sync controller mappings DB with SDL 2 community repo Synced with mdqinc/SDL_GameControllerDB@4c9b8dace8378e22474556e0259d02b59ea55484 (cherry picked from commit 6772047e5077b3bfe1f81cd24c128f02b904c20e)
- enet: Sync with upstream commit c44b7d0 https://github.com/lsalzman/enet/commit/c44b7d0f7ff21edb702745e4c019d0537928c373 (cherry picked from commit 360a1a456818fce75a444c17bafb9c7b510dbc20)
- certs: Sync with Mozilla bundle as of Mar 11, 2024 https://github.com/bagder/ca-bundle/commit/c5a419971b1bec220368c619aaafd0b818aa119f (cherry picked from commit fd61a42e0420c80481960a4071275f429186862b)
- mbedtls: Update to upstream version 2.28.8 (cherry picked from commit 915ca4dd456f959e4d4fd4e385715f3f0d48e77d)
- libpng: Update to 1.6.43 (cherry picked from commit 41268d7faa05888f0067679fa3374138de2f073b)
- tinyexr: Update to 1.0.8 (cherry picked from commit ab14dec952f0210448b6a2575cc0f73e7a074440)
- miniupnpc: Update to 2.2.7 No change for the files we include. (cherry picked from commit afe64bb517a83faf931bc13094220f85f6789f82)
- miniupnpc: Update to version 2.2.6 (cherry picked from commit 72b80b3cdfb90dbaad3ebe445eb7a3002f97471a)
- Physics interpolation - Zero server side multimesh data To prevent possibility of use of uninitialized data.
- Tighter light culling - fix directional lights colinear case Exactly the same fix as done already for non-directional lights.
- Don't define NO_EDITOR_SPLASH in export templates
- Fix typo in @GlobalScope.xml "inteters" instead of "integers"
- Fix long category name display in Inspector
- Fix Viewport interpolation mode Viewport interpolation mode is a special case, which should be set to ON instead of INHERIT.
- Physics Interpolation - Fix `VisualInstance::set_instance_use_identity_transform()` The logic for updating the `VisualServer` with the transform was the wrong way around.
- Fix fragcolor write locations in scene shaders
- Tight shadow culling - increase epsilon to prevent flickering Near colinear triangles were still causing inaccuracy in culling planes, so the threshold for colinearity is bumped up.
- Fix theme item parameter completion
- SCons: Fix Windows/MinGW TypeError with recent SCons Fixes #86484.
- Add range hint for ViewportContainer.stretch_shrink
- GDNative: Fix Linux riscv warning about ignored `sysv_abi`
- Optimize Editor `_init_grid()` Use static `LocalVectors` instead of `PoolVectors` for temporaries.
- ImageTexture - document workaround for mipmap generation
- Mesh merging - refactor to be backward compatible for CPU / GPU storage Allows the old `merge_meshes()` function to work from the editor.
- Fix physics tick counter The counter is now incremented at the start of a physics tick rather than the end.
- Physics Interpolation - Fix `Transform2D::interpolate_with()` Ports the `interpolate_with()` routine from 4.x which works correctly with skew.
- Fix AtlasTexture::draw_rect flipping for non-zero margin
- Fix type hints in GLTFDocumentExtension virtual methods
- Fix TabContainer not updating content rect after toggling tab icon
- Backport fix documentation about body_shape_index
- FIXED: Locale fallback selector doesn't appear in project options.
- Tilemap editor - prevent changing tool when mouse buttons pressed 
  Changing tool when painting prevented the corresponding commit of undo action when the mouse button was released. This led to undo actions getting out of sync and the undo system breaking the editor.
  This PR simply prevents changing tool while mouse buttons are pressed, and prevents the above scenario.
- Allow LSP to process multiple messages per poll (cherry-picked from commit e2485044a1b33628e6149d4f930b6fe065743c9d)


## [4.3.0]

Highlights:

- Backported the new TileMap from Godot4. Every class from it has been prefixed with the word `Layered`, so look for the `LayeredTileMap` class. The old `TileMap` is still available without any breakages.
- Created a new VertexLights2D module.
- Created a new VertexLights3D module.
- Better LOD support due to a change from godot3.x.
- Backported PROPERTY_USAGE_ARRAY from Godot 4.
- Fixed, set up exporter and export templates for the FRT and FRT SDL platforms (it's for `SBC`s).
- Fixed IOS export template .
- Now WebServerSimple can also use temporary files for uploads.
- Added a simple HTML Templating solution. See the HTMLTemplate and HTMLTemplateData classes and [this demo]($DOCS_URL/07_demo_projects/web/users_sqlite_bootstrap_templated).
- Build containers and release setup rework.
- ... and many more 

Breaking changes:

- Android dependencies had to be updated, unfortunately this raises the required java version to 17. On the flip side hovewer there is a new editor java home setting, and also the JAVA_HOME environment variable gets picked up.
- Build containers now use docker instead of podman.
- Removed parse_files() from WebServerRequest. It's doesn't work well with how the http protocol works. It did not do anything. If you called it just remove that call.

### Added

- Added FRT and FRT SDL build containers.
- New icon for frt sdl.
- Added an exporter for frt_sdl.
- Added a separate exporter for FRT.
- Added entity_resource_base_world_speed property to the ESS singleton.
- Also add is_zero_approx() to Vector4.
- Added copy pose and paste pose tools to the skeleton editor. Also added separators to it's dropdown menu.
- Make RAO and VertexLights2D work with the new LayeredTileMap setup.
- Created a new VertexLights2D module.
- Created a new VertexLights3D module.
- Added exp2 to the Math singleton.
- Added Material and merger support for PropDataMergeGroup.
- Added Material support for PropDataProp.
- Added a new PropDataMergeGroup Node for props.
- Implemented RAO for LayeredTileMaps.
- Added a new canvas_item_add_texture_rect_animation() method to the RenderingServer.
- Proper docs for WebServerSimple.
- Updated docs for FileCache.
- Added optional force parameter to WebServerRequest::move_file(). Also various fixes and improvements to the new upload temp file system.
- Added new wwwroot_get_simplified_abs_path() helper to FileCache. Also improved the other path helper methods.
- Now WebServerSimple can also use temporary files for uploads.
- Added move_file() and is_file_moved()  helper methods to WebServerRequest.
- Added docs for String::substr_index().
- Added more file formats to the Text Editor's Save Dialog.
- Added HTMLTemplate and HTMLTemplateData classes.
- Added a new method to Expression so it can be used with multiple threads simultenously.

### Fixed

- Fix crossbuilding libtheora on the server platform.
- Fixed building FRT.
- Fixed building FRT SDL.
- IOS build scripts.
- Fix MMImage.
- Hide the FileSystemDock's empty dock slot when it's in bottom bar mode by default.
- Use internal process for updating a dirty skeleton instead of the MessageQueue. This fixes BoneAttachments being a frame late.
- Sync NativeScript's PROPERTY_USAGE flags with Object's.
- Fix 2d mesh transforms and colors.
- Various doc fixes.
- Fix 2 off-by-one errors.
- Fix incorrect clipping in String::substr_index().
- Use String's length() instead of size().
- Fix class link in the docs for SMTPClient.

### Changed

- Moved the old frt build containers.
- Renamed the build container folders.
- Renamed uwp's exporter's folder so it's not in the editor while that platform cannot yet be built.
- Reworked the build containers and the in-container build scripts.
- Added an exported for the server platform.
- In-container build script cleanups.
- Build containers now use docker instead of podman.
- Updated the build containers based on godot's.
- Now the input_properties and output_properties property won't get saved directly in MMNode (this was the original intent, nothing should change other than MMMaterial files should be a bit smaller now.). They are still visible in the editor.  Also make sure an MMNodeUniversalProperty can only be registered once.
- Make mat maker's image picker button's dialog use resources.
- Fix error found by static analysis tools.
- Improved Light setup in PropInstance.
- Added more properties to PropDataLight and other light helper classes.
- Now CharacterSkeleton2D inherits from YSort. It disables sorting by default, so the default behaviour won't change.
- Now tooltips won't get scaled to their original control's scale anymore by default. Added a new gui_scale_tooltips property to Viewport to be able to use the previous behaviour when it's desired.
- Removed clips_input() method and _clips_input() virtual method from Control. Made clip_contents to also have an input clipping effect, since I couldn't think any single instance where keeping them separated would be useful. This fixes LayeredTileMap's editors clipping input while being invisible, and likely other smaller issues.
- Now EditorPropertyEnums can be setup multiple times.
- Improve multipart filename handling a bit.
- Now SimpleWebServerRequest Only count request size when not writing to a file.
- get_file_length() in WebServerRequest now returns uint64_t.
- Updated sample module_config.py.
- Use bitshifts for all property usage flags.
- Use PROPERTY_HINT_BUTTON in UserManagerStatic instead of a bool.
- Now PROPERTY_HINT_BUTTON adds the property's name as the called method's first parameter.

### Removed

- Removed parse_files() from WebServerRequest. It's doesn't work well with how the http protocol works.
- Removed Vector2i(Vector2) constructor, added a Vector2i conversion operator to Vector2 instead. This solves ambigous Variant to Vector2i conversion errors.

### Backports

- Backported everything up to and including https://github.com/godotengine/godot/commit/90cf873979a0e5b13a2cbc97eb077ee9266e48a5 Merge commit: https://github.com/godotengine/godot/commit/a73715cf55d6709655ae9563e0f6e4f4b12b9955

#### Godot3

- Fix AudioStreamPlayer not paused on pause mode change Fixes #58543.
- Add option to toggle visibility of position gizmos in 2D editor.
- Portals - defer setting active in `VisualServer` until enter tree `set_portal_active()` was being called loading packed scenes prior to entering the tree, visual server portals had not been fully created at this point hence the call was being ignored with an error flagged. This PR defers the call until after entering the tree.
- Add option `modules_enabled_by_default` and minimal CI build.
- Fix physics on_floor_body crash Physics body previously stored the RID of a collision object and accessed it on the next frame, leading to a crash if the object had been deleted. This PR stores the ObjectID in addition to the RID, and checks the object still exists prior to access.
- Store ObjectID instead of pointer for KinematicCollision owner
- Fix SceneTree not respecting virtual process methods
- Don't error multiple times when trying to load missing default bus layout
- Backported Update Android dependencies for the project
  Update Java version from 11 to 17
  Update Android gradle plugin version from 7.2.1 to 8.2.0
  Update gradle version from 7.4.2 to 8.2
  Update target SDK from 33 to 34
  Update build tools version from 33.0.2 to 34.0.0
  Update kotlin version from 1.7.0 to 1.9.20
  Update Android fragment version from 1.3.6 to 1.6.2
  Update AndroidX window version from 1.0.0 to 1.2.0
  Update Nexus plugin version from 1.1.0 to 1.3.0
  m4gr3d
  https://github.com/godotengine/godot/commit/eba77be573793243a91322c7eb8e345695c3b813
  Unfortunately this raises the required java version to 17. On the flip side hovewer there is a new editor java home setting, and also the JAVA_HOME environment variable gets picked up.
- Add is_zero_approx methods to Vector{2,3}
- Check duplicate keys in dictionary literals: enums and const variables
  Check identifiers (const variables and unnamed enums) and named
  enums when parsing dictionary literals whether the keys are not
  duplicated.
  In case of duplicate key is encountered, highlight the line with it 
  and print error message:
  `Duplicate key "foo" found in Dictionary literal`
  This commit is a logical continuation of the commit dab73c7 which
  implemented such checks only for literal keys (which fixed #7034).
  Apart from that, this commit also fixes the issue with the error
  message itself, which was shown one line below the duplicated key
  in case it was the last one in the dictionary literal and there
  was no hanging comma.
  Also, the format of the error message has been changed so that now
  the error message also contains the value of the key which is duplicated.
  Instead of `Duplicate key found in Dictionary literal`, it now prints
  `Duplicate key "<value>" found in Dictionary literal`
- Tighter shadow culling - fix light colinear to frustum edge
  In rare situations if a light is placed near colinear to a frustum edge, the extra culling plane derived can have an inaccurate normal due to floating point error.
  This PR detects colinear triangles, and prevents adding a culling plane in this situation.
- Add Node.is_node_ready
- nanosvg: Sync with upstream 93ce879
  https://github.com/memononen/nanosvg/commit/93ce879dc4c04a3ef1758428ec80083c38610b1f
- Backported ORMSpatialMaterial from Godot 3.x without any breaking changes.
- FIXED: Trying to connect inexistent signal text_submitted.
- Backport locale selection improvements.
- Fix building with `disable_3d`
- Fix SceneTree dock filter crash. The filter was crashing for two reasons: 1) Deleting a child invalidated the iteration of children 2) Child was accessed after deletion
- Rotary input for 3.x
- Fix GodotEditText white box showing during game load
- Discrete Level of Detail
  Add scene side discrete level of detail.
  New node `LOD` for UI, and `LODManager` within `World` for automatically updating child visibilities based on distance from cameras.
- Remove deprecated LaunchImages support and switch to Storyboard by default.
- Fix finding AnimationPlayer in scene import
  The scene importer always assumed that the AnimationPlayer is called
  "AnimationPlayer".
  This is not always true: for example the GLTF importer just creates an
  AnimationPlayer with the default name, which may be "animation_player",
  depending on the project settings.
  This fix instead chooses the first node that is an AnimationPlayer, and
  warns if there is more than one.
- Physics Interpolation 2D - fix light and light occluder resetting
  It turns out `NOTIFICATION_TRANSFORM_CHANGED` is deferred for these nodes, which can mean the transform is not set in the `VisualServer` until after the reset has been sent, even if the transform is set before the reset in script. This prevented the reset from acting correctly.
  Here we explicitly set the transform prior to each reset, to ensure the `VisualServer` is up to date.
- Optimize `AnimationTree::_process_graph()` Removes redundant lookups on HashMap.
- Allow automatic provisioning in iOS when using development signature. Implements https://github.com/godotengine/godot/pull/86748/files in 3.x.
- Physics Interpolation 2D - change transform API to use const ref
- GDNative: Fix Linux arm32 warning about ignored sysv_abi attribute
- Add a `get_or_add` method to Dictionary
- Fix `Mesh::get_face_count()`
  This fixes a minor bug whereby facecount was actually returning the facecount * 3.
  There were no major problems from this, but it did mean the optional threshold poly count used when merging was out by a factor of 3.

#### Godot4

- Backported from godot4: Save PackedByteArrays as base64 encoded. - groud https://github.com/godotengine/godot/commit/de5073519eb8085a1b0f9736a44dfe3e4ae1c561
- Backported from godot4: [TileMap] Fix forcing cleanup on exiting tree/canvas - AThousandShips https://github.com/godotengine/godot/commit/09edece17d837f13f23aab13a546fe3e97a6c223
- Backported from godot4: Fixes "no cached rect" errors in TileMapLayer editor - groud https://github.com/godotengine/godot/commit/c928273c6c74dbbe3a21f9978ffdfa3fb03ea547
- Backported from godot4: Correctly update TileMapLayer highlighting when disabling it - groud https://github.com/godotengine/godot/commit/256a6713f22d56b4a42703919f1e4718b95de9e6
- Backported from Godot4: Expose TileMapLayer - groud https://github.com/godotengine/godot/commit/3cd4b2859c9d715459325b070643773b47472324
- Backported dome of the codestyle improvements to math types from https://github.com/godotengine/godot/commit/3fb36bf395f2cc03ca8bb060d0afdb412c0855b4 .
- Backported from Godot4: Allow disabling scrolling in Tree. - groud https://github.com/godotengine/godot/commit/b2dddc3c82e166cdca715e7f66eebf21be79f134
- Backpoered from Godot 4: Add is_built_in() method to Resource. - KoBeWi https://github.com/godotengine/godot/commit/e393c2a7348d96587e15af17c72ebeff719d4be3
- Backported from godot4: Use a SpinBox in the inspector array resize dialog. - fire-forge https://github.com/godotengine/godot/commit/d6188b46328bbfde3e7e97174e4a7fca05c90b20
- Backported from godot 4:  Reduce contrast in inspector array element backgrounds. - fire-forge, reduz https://github.com/godotengine/godot/commit/f26414762f33c660083078d3e1d10bbf6e1039a3
- Backported from godot4:  Defer EditorInspector::update_tree to the process stage to improve performance `EditorInspector::update_tree` is expensive, so defer the call to the process phase to prevent multiple calls in a single frame (when switching scene tabs). - Rindbee https://github.com/godotengine/godot/commit/f352c30ad59dc50d13bb979a63271b668fc39c5b
- Bakcported a change to the EditorInspector from: Implement polygons editors in the tiles selection mode - groud https://github.com/godotengine/godot/commit/cec004adf08fc57cd1d27408ee5b67637355f2e6
- Backported from godot4: Fix property array tooltip shows wrong ID on later pages. Also do some cleanup, use`element_position` instead of `begin_array_index + i` - jsjtxietian https://github.com/godotengine/godot/commit/d4ac65bc48aeda1e22a78c017110951a6aa4444b
- Backported parts of: Clean-up array editing - reduz https://github.com/godotengine/godot/commit/0351a0908f87e0a67a2a81500275388a4c62ddfe And: Fixup array property inspector. - rburing https://github.com/godotengine/godot/commit/4a59188898284e583e66c3a5c9cd3969730ad2d4
- Backported from Godot4: Implement Tabs minimum size. - groud https://github.com/godotengine/godot/commit/898a2a7cf3e39c37840749581e7683f37a3e011c
- Backported min and max zoom setting to the EditorZoomWidget from godot 4.
- Backported comparison operators to Array from godot4.
- Backported PROPERTY_USAGE_ARRAY from Godot 4. Reused one of the old deprecated property usage flags for it. Original commit: Implement properties arrays in the Inspector. - groud https://github.com/godotengine/godot/commit/4bd7700e89f4476a3b2c76e7bed4263a111b3c09
- Backported goodies to SelfList from Godot4.
- Backported ViewPanner from Godot4. It also got bindings, now inherited from Resource, and is registered in the ClassDB.
- Backported create_reference() helper methods for InputEvents from godot 4.
- Backported godot 4's tilemap as a new layered tilemap module.

## [4.2.1]

### Added

- Added get_global_class_name method to Script. Now GDScriptParser uses this first to determine global class names, instead of parsing the script file again.
- Added a way for platforms to define a custom resource pack location.
- Added more defines for platform specific configs.
- Added S3TC_NOT_SUPPORTED define to be used by platforms externally.
- Added optional GLES2_LOAD_EXT_NO_DLCFN_AVAILABLE define for the rasterizer storage.

### Changed

- Turn custom module paths relative to the engine's directory when they reside inside the engine's folder.

### Fixed

- Found and updated some remaining old Java package paths.
- Fix checking the wrong property in the Android's export plugin.
- Moved dlfcn.h include in RasterizerStorageGLES2 to it's proper place according to the comment above it.
- Fix custom module paths for the editor builds.
- Small tweaks to HTTPServerSimple, mostly related to ssl handling.
- Fix FileCache for files in .pck-s.
- Fix DirAccess::get_filesystem_abspath_for() for .pck files.
- Fix build on MSVC.

## [4.2.0]

### Added

#### Core

- Added missing setters to VariantOP.
- Added to_real helper methods to String. (Also added it to the gdnative api.)

##### PLogger

- Added log level support for PLogger.

#### Servers

- Added a new dummy WindowServer class. (Disabled it for now.)

#### Editor

- Added back ResourceImporterOBJ from godot.
- Added a new editor setting (docks/filesystem/wide_bottom_panel) which makes the file system appear in the bottom panel instad of as a dock.
- Now the names of the main screen plugins can be hidden in the editor by the new interface/editor/hide_main_screen_plugin_names editor setting. (If a plugin does not have an icon fall back to using it's name.)

#### Scene

##### GUI

- Added a new CSplitContainer type which acts as both a VSplitContainer and HSplitContainer and allows changing between them using a property.
- Added a new CBoxContainer type which acts as both a VBoxContainer and HBoxContainer and allows changing between them using a property.

#### Modules

##### SMTP

- Added a new smtp module.

##### Tile Map

- Added a button to the TilesetEditor to easily select the TilesetEditorContext when needed.

##### Web

- Implemented range requests for the HTTPServerSimple. It does work in fiefox, however wget seems to choke on it. This will be fixed later.
- Added a way to get all available parameter keys to WebServerRequest.
- Added a way to unregister connections and get out their raw connection data from the http server.

##### Prop

- Implemented PropDataStaticBody handling for TerrainPropJob and VoxelPropJob.
- Initial StaticBody support for the prop system.

#### Misc

- Added a backport helper script.

### Fixed

#### Core

- Fixed smaller issues in VariantOP.
- Fix running standalone scripts in non-editor builds.
- Fix logic in String::substr_index.
- Added missing Variant PTRARG conversion.

#### Modules

- Fix build when the mbedtls module is disabled.
- Fix build when the jsonrpc module is disabled.

##### Editor Text Editor

- Implemented clearing custom fonts in the editor's text editor module. Also small improvements.
- Fixed custom font support in the editor's text editor module.
- Fixed reopening files from the previous session in the editor's text editor module. Also small cleanups.
- Added more extensions to the editor's text editor module.
- Fixed all the crashes (and potential crashed) in the editor's text editor module I could find. Also fixed some smaller usability issues.
- Fix lots of potential crashes in the editor's text editor module. Now it also won't add an additional newline to the end of files.
- Removed the delete file option from the editor's text editor module.
- Disabled syntax highlighting in the editor's text editor module. It will need to be reworked.
- Fixed TextEdit width calculation after it's font changes. This fixes lines being too long to scroll to in the editor.

##### Web

- Fixed generating urls in BrowsableFolderServeWebPage.
- Now PagedArticleWebPage won't add the summary page to the list of pages you can switch to.
- Store incoming http request parameters properly.

##### GDNative

- Mark NativeScripts as having delayed metadata.

#### Docs

- Fix the doc descriptions where they got lost due to adding StringName to Variants. Also fixed smaller issues where I ran into them.

### Changed

#### Generic

- Some reorganization of files, particularly in the scene folder.

#### Core

- Updated year in version.py.
- Allow running standalone scripts in non-editor builds.
- Optimized path_clean_end_slash in String.
- Now unicode error printing is disabled by default. Also can be enabled via a macro.

##### PLogger

- The default log level in PLogger is now message.
- Call the proper methods in PLogger. Also use ERR_PRINT when logging errors.

#### Platform

- Updated copyright info in pandemonium_res.rc.

#### Editor

- Bind more methods in EditorInspector.
- Don't hide the script property in sub inspectors.

#### Modules

- Updated the sample module config file with the new modules.

##### Navigation Mesh Generator

- Don't print error when there is no default NavigationMeshGenerator.
- Don't warn when falling back to NavigationMeshGeneratorDummy.

##### Network Synchronizer

- Now SceneSynchronizerDebugger can properly create it's debugger.py file.
- Updated NetworkSynchronizer from https://github.com/GameNetworking/network_synchronizer/tree/godot-3.x .  f12fee66a7e3eb2013d6c5437837770455105b91 (Merge commit: cc3c7d244adea8b25822f43963618fd2dbf18ec8)

##### Tile Map

- Renamed RTilesetEditorContext to TilesetEditorContext.

##### Web

- Completely removed the cached path api from FileCache.

#### License

- Updated the license to current year, and also updated it on the top of the headers.

### Backports

#### Godot3

- Backported everything up to and including https://github.com/godotengine/godot/commit/b859a1898d82ff5d0c568d45b45eabc054d981cf Merge commit: https://github.com/godotengine/godot/commit/cc4492f9b9ccb8d93c132d97ca3439b8ba925b08

- Ported:  Update the logic used to start / stop the GL thread Currently the GL thread is started / stopped when the activity is respectively resumed / paused. However, according to the `GLSurfaceView` documentation, this should be done instead when the activity is started / stopped, so this change updates the start / stop logic for the GL thread to match the documentation. - m4gr3d https://github.com/godotengine/godot/commit/194452bf38e5a0de0f1c8e0698244a6f47b88a08
- Ported:  Vertex cache optimizer Optimizes indices to make good use of vertex cache on GPU. - lawnjelly https://github.com/godotengine/godot/commit/0aa22b8f1309652353a1ca257f75ff6b3a0437c8
- Portred: Add MergeGroup node to simplify merging Meshes at runtime. - lawnjelly https://github.com/godotengine/godot/commit/8b791355386b9528523010d27e9a674b5bf8c28a
- Ported:  Fix signed distance field font rendering This fix works in both GLES3 and GLES2. The rendering formula in the shader was adjusted to further improve the sharpness/antialiasing quality balance. - lawnjelly and Calinou https://github.com/godotengine/godot/commit/bc607fb6075cebf78d8e562d7b7423b2650906ac
- Ported:  Shadow volume culling and tighter shadow caster culling Existing shadow caster culling using the BVH takes no account of the camera. This PR adds the highly encapsulated class VisualServerLightCuller which can cut down the casters in the shadow volume to only those which can cast shadows on the camera frustum. This is used to: * More accurately defer dirty updates to shadows when the shadow volume does not intersect the camera frustum. * Tighter cull shadow casters to the view frustum. Lights dirty state is now automatically managed: * Continuous (tighter caster culling) * Static (all casters are rendered) - lawnjelly https://github.com/godotengine/godot/commit/8ca631a466e3dfe0d6e0bce54c6a78c652fca81c
- Ported parts of:  [3.x] Add Selected Collision Group in TileSet Editor Also moves the TileSet Script property down to its own category. - Mickeon https://github.com/godotengine/godot/commit/211c707e67725f27ae995a923dd206cca9976315
- mbedtls: Update to upstream version 2.28.7
- certs: Sync with Mozilla bundle as of Dec 13, 2023 bagder/ca-bundle@bef37a9
- Sync controller mappings DB with SDL2 community repo Synced with mdqinc/SDL_GameControllerDB@232c738
- Fix build on X11 following 64-bit detection changes This also ports over the cross-compilation logic to the `server` platform, and allows Embree to be used in server tools builds on aarch64.
- Fix invalid Python escape sequences
- SCons: Fix Python 3.12 SyntaxError with regex escape sequences
- Linux: Remove hardcoded lib path for x86 cross-compilation This breaks the build with our updated i686 Linux SDK which doesn't contain this path, and may not be needed at all.
- CI: Update mymindstorm/setup-emsdk to v14, should fix cache folder conflicts https://github.com/mymindstorm/setup-emsdk/releases/tag/v14
- Style: Mark clang-format 16 as supported for pre-commit hook It only introduced a difference in a .glsl file, which I've worked around by removing an empty line. This keeps formatting consistent between clang-format 15 and 16.
- SCons: Fix Web build when compiler version isn't properly detected Quick fix for #82585. Also set CI version to 3.1.39, which is what we use for official 3.6 builds since 3.6-beta4.
- Portals - Improve conversion logging Logging is now allowed in any TOOLS build (rather than just in the editor), but still prevented in final exports. Logging  an be switched off via project settings. Autoplacement is now logged.
- Portals - include in bound and special cases in start room * Re-introduces a property for portals to decide whether they are included in room bounds during room conversion. * Adds a special case for portals that extend into the start room, which may be caused by level design inaccuracies.
- zlib/minizip: Update to version 1.3 Remove `infback.c` which we don't need. The `OF` macro was also removed so I can drop the patch where I yell at Gentoo developers.
- minizip: Backport patch to fix CVE-2023-45853
- brotli: Sync with upstream 1.1.0 https://github.com/google/brotli/releases/tag/v1.1.0
- Web: Clarify that `OS.get_unique_id` is not supported Remove the base error message in `OS`, we no longer really error out this way for not implemented methods. Instead, each platform should override them to provide the context they want.
- Add XInput device ID for wireless Series 2 Elite controller
- SCons: Add `stack_size` and `default_pthread_stack_size` options to Web target
- Set what were default values for some emscripten linkflags - Set `-sSTACK_SIZE` to what it was before emscripten 3.1.27. It was renamed in 3.1.25 so also set  sTOTAL_SIZE for older versions for consistency. And Set `-sDEFAULT_PTHREAD_STACK_SIZE` to what it was before 3.1.30.
- Fix generating vsproj with SCons 4.6.0+
- Linux: Add support for arm64 and arm32 export templates This is done in a hacky way, mostly to keep it simple and avoid having to do a refactoring of the `EditorExportPlatform` interface.
- GLES2 / GLES3 - Use gl_FragColor temporary On some hardware, modifying gl_FragColor multiple times can cause large performance drops. This PR writes to a standard temporary variable instead, and copies across to gl_FragColor once only at the end of the fragment shader. This could potentially lead to large gains in performance on affected hardware.
- Mention InputEventJoypadButton's pressure not working
- Add Editor Description group Backport from 4.0
- GDScript: Fix `get_method_list` for custom functions
- Add APP_PAUSED and APP_RESUMED MainLoop notifications
- Prevent shuffling custom shader functions (shader cache requires determinism)
- Document some Image methods can unlock it (making `set_pixel` fail)
- Improve Scene Tree Dock's Node filter (Allow multiple terms & more)
- Preserve selection when focusing SpinBox
- Rewrite most of Resource's documentation
- Update linker flags for Xcode 15.0 - 15.2. Bump min. iOS version to 12.
- UWP: Fix VS 2017 build with new `get_unique_id` method Fixes this error:```platform\uwp\os_uwp.cpp(715): error C3149: 'Windows::Storage::Streams::IBuffer': cannot use this type here without a top-level '^'```
- Fix `#if *_ENABLED` inconsistencies, should check if defined
- Fix Dummy audio driver initialization issue on WASAPI output device initialization failure
- Add project settings for AVAudioSessionCategory on iOS
- Fix AABB.encloses failing on shared upper bound This differs from `Rect2` and was fixed for those classes in the past
- Implement UWP version of `OS.get_unique_id` function.
- Prevent using name with leading dot when create/rename/duplicate scene/folder/script/resource Fixes #62497
- Fix unitialized variables in `core`
- Fix typo.
- Remove unused struct in GradientTexture1D
- GDScript: Prevent native class shadowing
- Add Android project settings for gesture support - Include project setting to enable long press for Android devices - Include project setting to enable pan and scale gestures on Android devices
- Improve TileMap editor visible names and tooltips
- GDNative: Fix Linux arm64 warning about ignored `sysv_abi` attribute Fixes #41160.
- Fix various GCC 13 warnings Fixes occurrences of `-Wtype-limits`, `-Wmaybe-uninitialized`, `-Wduplicated-branches`.
- Update the `launchMode` for the `GodotApp` activity to allow other activities to be part of the same task For details, see https://developer.android.com/guide/topics/manifest/activity-element#lmode
- Fix Android editor crash issue Fix issue causing the Android editor to crash when pressing back from a running project
- Automatically pick the Android sdk path using environment variables
- Also backported the previous change for cscript.
- Far faster and more efficient method of checking if an identifer refers an autoload.
- Make gizmo plugin handle SpriteBase3D instead of Sprite3D
- Fix grayscale dds loading
- Fix glGet overflows by using 64 bit versions Certain glGet operations require 64 bit versions according to the GLES spec. The previous code was susceptible to overflow bugs,  especially running under ANGLE.
- Remove the separator from `ItemList`'s thumbnail mode
- Use occlusionState to determine when window is drawable.
- Return exit code 0 when running --version or --help Fixes https://github.com/godotengine/godot/issues/83661
- Fix File.get_buffer returning wrong length File.get_buffer always returned as many bytes as requested (even when EOF was reached), this resulted in random bytes being returned when overflowing.
- Make OSIPhone::get_screen_refresh_rate respect iOS Low Power Mode
- Allow dragging editable children
- Workaround GCC warning in `rasterizer_canvas_batcher` `-Werror=array-bounds` flags when creating a new batch, possibly due to the possibility of the malloc failing (out of memory). This PR adds an explicit `CRASHNOW` in the hope the compiler will recognise this case is not intended to be recoverable.
- mbedTLS: Update to version 2.18.5
- Check parameter validity in `Object::set_script` Fixes #46120.
- Updated controller mappings DB from godot.
- Web: Fix version check for missing scalbnf LTO workaround The check needs to happen after we set `env["CXX"]`. Follow-up to #81340.
- Web: Workaround Emscripten 3.1.42+ LTO regression Fixes #80010.
- Updated github action checkout.
- Build system: add option for MSVC incremental linking.

#### Godot4

- Fix TextEdit.get_rect_at_line_column returning negative pos even though cursor is in viewable area of the control - MJacred https://github.com/godotengine/godot/commit/6170381bd754490aee34e5a34558460d5c2f33f9


## [4.1.0]

### Added

#### Core

- Added back the translations from godot.
- Added back missing descriptions to Translation's class docs.
- Added translate_to() method to TranslationServer, and trt() method to Object. This let's you transalte strings into specific (noon app wide) locales.
- Added missing Variant conversion and zero cases.

#### Docs

- Added docs for StringName.

#### Platform

##### OS

- Added a new SubProcess class with a Unix and Widnows backend for better process control.
- Updated an another missing OS.run() call.
- Added run, create_process and create_instance helper methods to OS. Inspired by godot4's split of execute.

##### UWP

- Added back the uwp platform. It likely won't work for a while. Fixed includes and build.

#### Scene

##### GUI

- Added Toplevel property to Node2D.
- Backported _shortcut_input() virtual from godot4.

#### Editor

- Added shortcut for toggling the default bottom menu items. CTRL + ALT + 1 - 5.
- Change the bottom bar collapse shortcut to CTRL (CMD) - U.
- Added a shortcut and button to quickly collapse / close the bottom panel (CTRL-B).
- Backported the new output window from godot4.
- Now custom resources can be exported from/to any scripting language.

#### Modules

##### Users

- Implemented getting users using their email from the UserDB.

##### GDScript

- Added setting to enable / disable gdscript's language server.
- Added back the language server for gdscript.
- Implemented a literal syntax for NodePaths in GDScript I decided on using godot4's (^). The doc already said it's '@' but that was wrong.

##### JSonRPC

- Added back the jsonrpc module.

##### Navigation

- Added init and finish methods to the NavigationMeshGenerator. Also small tweaks in initialization.
- Added finish method to the NavigationServers.

##### TileMap

- TileMaps now support navigation map overrides.

##### Paint

- Added a new PaintPolygon2D and PaintCurve2D Nodes.

##### Web

- Added more mime types by default.
- Added start_on_ready property to WebServerSimple.
- Added a way to customize the doctype declaration the html tag and the body tag to WebServerRequest.

#### CI

- Added a note and some commented out commands to the container linux build script.
- Save artifacts for both the server and the http server github actions.
- Added a http server template build github action.

### Fixed

#### Core

- Fix typos in _rpc_id_bind and _rpc_unreliable_id_bind.
- Make sure rpc binds can take both Strings and StringNames as method names.
- Only update the RenderingServer from the Main Thread while in debug break.

#### Docs

- Replace navigation demo locations.
- Updated the links in the docs.
- Changed the docs url from godot's to my docs repository.

#### Platform

##### Android

- Fix JNI method names and java class lookup names in the android backend.
- Fix the android lib's package locations.
- Fix the android editor's package locations.

#### Scene

##### GUI

- Set input as handled when processing shortcuts in BaseButton. This fixes the issue when the script editor tab is active and the scene - tree editor is focused, a ctrl+a press both opens the new Node popup and selects all text in the script editor.

##### Navigation

- Bind missing method in NavigationServer.
- Fix NavigationObstacle2D::set_navigation.
- Also add docs for the newly bound methods.
- Fix the initial value of _debug_enabled in NavigationServer.
- Fix threaded bake logic in NavigationPolygonInstance and NavigationMeshInstance.
- Bind get_navigation_map and set_navigation_map in NavigationObstacle2D.
- Simplify some logic NavigationObstacle2D.

#### Modules

##### Navigation

- Make sure that all parameters are properly set into PathQueryResults.
- Fix querying paths.

##### GridMap

- MeshLibrary's SceneConverter will also processes the navigation layers for the items.
- Use get_navigation_map() in GridMap.
- Fix logic in GridMap::set_navigation_map().

##### Web

- Fix counting children even if the server is running in WebServer::get_configuration_warning().
- Fix crash in WebServerRequest::get_url_root().

### Changed

#### Editor

- Prefix messages with their thread id if they aren't coming from the main thread in the editor log.
- The match case option is on by default now when searching in the current file and in files in the editor.
- Removed the text from the layouts buttons in the CanvasItemEditor.
- Replaced the text with an icon on the SpatialEditor's and the CanvasItemEditor's view button.
- Replace the Transform text with an icon in SpatialEditor's toolbar.
- Removed superfluous VSeparator from the SpatialEditor.
- Convert MeshInstanceEditorPlugin's toolbar to the new style.
- Added separators to TerrainWorldEditor and VoxelWorldEditor's toolbars.
- Converted the menu buttons of the Portals and Rooms system to the new style.
- Small tweak to the tooltip of the quick prop convert buttons.
- Never show text for the Convert Rooms button.
- Use an icon for the quick prop conversion buttons instead of text.
- Added a VSeparator to the To Prop editor buttons.
- Group the Anchor and Presets buttons under a common HBoxContainer in CanvasItemEditor.
- Added a VSeparator to the NavigationPolygonEditor and the NavigationMeshEditor.
- Removed the diffferently styled context menu from  the SpatialEditor and the CanvasItemEditor. New controls are now added directly to the main FlowContainer, so they wrap properly. While having a different background for context sensitive items can look good, it doesn't work well if it's not per plugin.

#### Modules

- Now VoxelWorld and TerrainWorld inherits from Spatial instead of Navigation, and Terrain2DWorld inherits from Node2D insteaf of Navigation2D, as there is now a proper NavigationServer. Also this will make Node hieararchies involvig them more versatile.

##### CScript

- The CScript module is now disabled by default. It will get re-enabled once it gets finished.

##### Navigation

- Removed cell height getters and setters from the Navigation2DServer.
- Simplifications to NavigationAgent and NavigationAgent2D.
- Bake the NavigationMesh on the main thread in the editor.

##### Web

- De default mime type is application/octet-stream now for files in HTTPServerConnection.
- Now HTTPServerSimple sends files in non-blocking mode.
- Send the user's header when sending the file in HTTPServerConnection.
- The mime types dictionary now uses StringName indexes. Also compare against lowercase extensions.
- Changed the default bind host in WebServerSimple to 0.0.0.0 for convenience.

### Removed

#### Generic

- Removed the entire funding.yaml file.
- Removed funding links.

### Backports

#### Godot3

- Backported everything up to and including https://github.com/godotengine/godot/commit/ab76395144baa431865ef2b39c6c9cfa11b8f154 Merge commit: https://github.com/godotengine/godot/commit/fe7ed984b5a187f0fbdf531fa72f7b7479778d79

- Port4ed from godot:  Add binary MO translation file support. Add brotli decoder and WOFF2 support. Use smaller .mo files instead of .po, if gettext is available. Convert editor fonts to .woff2 format. - bruvzg https://github.com/godotengine/godot/commit/fd2fba7c2cfa96b0987c3c9f12e304bb2bbd9183
- Ported from godot: Add context support for editor translation - timothyqiu https://github.com/godotengine/godot/commit/878cf8262a94367f4ffb7c706773075cd563ee38
- Ported from godot: add support for 3 dir shadow splits - Ansraer https://github.com/godotengine/godot/commit/54bc6a300e4c21579117c1b728042977ee8a1534
- Ported from godot:  CPUParticles2D - Add ability to follow physics interpolated target Allows a non-interpolated particle system to closely follow an interpolated target without tracking ahead of the target, by performing fixed timestep interpolation on the particle system global transform, and using this for emission. - lawnjelly https://github.com/godotengine/godot/commit/3e19cf834aa3116f2e8cc329d232b3e747becfb7
- Ported:  CPUParticles2D - fix interpolated transforms and culling 1) Physics interpolated particles in global mode are specified in global space. In VisualServer they should therefore ignore local transform. 2) Additionally, the expected final_transform should be passed on to children, rather than the identity transform used on the local item. 3) Local bounds in hierarchical culling are fixed for items using identity transform, by calculating their local bound in local space from the global space particles. - lawnjelly https://github.com/godotengine/godot/commit/723632a76a6d88c2f95ef728bbf2f7182cab05a0
- Ported from godot:  Fix Android input routing logic when using a hardware keyboard When a hardware keyboard is connected, all key events come through so we can route them directly to the engine. This is not the case for soft keyboards, for which the current logic was designed as it requires extra processing. - m4gr3d https://github.com/godotengine/godot/commit/620fdd1f078908e665a0eb2dd71306016c785abd
- Fix PopupMenu icon and text not having separation
- BVH - fix axis getting mixed up when split leaf
- BVH - fix not refitting upward from leaf nodes
- Workaround Xcode 15 linker bug.
- Change target version to 11.0+ to fix PCRE SLJIT build.
- Physics Interpolation - automatic resets for Camera2D and TileMap.
- CPUParticles - fix non-interpolated NOTIFICATION_TRANSFORM
- Disable live resize in multithreaded rendering mode.
- Mark debugger limits settings as requiring a restart
- Fix cursor after last character in line counting as a character outside of the viewing area
- mbedtls: Fix UWP arm32 build after 2.28.3 enabled AES-NI intrinsics on MSVC
- Make `TextureButton` and `Button` update on texture change
- Fix AudioEffectRecord circular reference
- Core: Add recursion level check for `VariantWriter::write()`
- Fix TouchScreenButton not redrawn when texture changes
- Fix JavaScript callback memory leak
- Revert "Backport implement loading DDS textures at run-time"
- Ported: Fix binary internal cache not being filled - Razoric480 https://github.com/godotengine/godot/commit/d331e5c7ac4865e9d023ebfbba1bc6473d0ecec6
- Ported: Force external resources to have their path set - Razoric480 https://github.com/godotengine/godot/commit/420794b61d2c77fe92ed390245b5c8467d30ef0b
- Ported:  Fix custom res caching sub-res even if no-cache -Razoric480 https://github.com/godotengine/godot/commit/2ceb93bbeff6f5c7b6eed4ea79aff5f0574f4f92
- Update the last backported commit.
- Actually use loaded resource cache
- Core: Add recursion level check for `Array` and `Dictionary` hashing
- wslay: Sync with upstream 0e7d106ff from godot.
- recast: Update to upstream version 1.6.0 from Godot.
- mbedtls: Update to upstream version 2.28.4  from Godot.
- SCons: Disable misbehaving MSVC incremental linking
- Ensure `joy_connection_changed` is emitted on the main thread
- Add missing stdint.h includes for GCC 13+
- Build `JoypadLinux` sandbox detection method only with udev
- Add support for DPAD Center key of Android TV remote controller
- tinyexr: Sync with upstream 1.0.7
- Update certs (2023.06 revision)
- certs: Sync with Mozilla bundle as of Mar 23, 2023
- Document the database for `Input.get_joy_name()` and `Input.get_joy_guid()`
- Added a few additional GUIDs to list of hardcoded IDs in is_xinput_device
- Add mono audio support to WASAPI
- Fix the Python type error when creating the .sln file
- add venv and .venv to the .gitignore
- Added note to used set_deferred while setting scroll values in _ready function.
- Fix double tap & drag on Android
- ios splash screen rotation fix
- Add information about how `Engine.time_scale` affects Timers
- Document custom mouse cursors larger than 128×128 potentially having issues
- Fix Windows StringFileInfo structure
- Fix moving position indicator out of bounds in FileAccessMemory
- Document how to use logarithm of base 10 with `log()`
- Fix scrolling behaviour with low page value
- Notify child controls when BackBufferCopy's rect changed
- Fix mesh library remove selected item menu option
- Added code coverate support to Godot
- Add static check for overzealous .gitignores.
- Remove too greedy GCOV ignores from `.gitignore`
- Fix: incorrectly .gitignored files.
- SCons: Enable `/WX` on LINKFLAGS for MSVC with `werror=yes`
- CI: Free disk space on Linux runners
- Update PCK embedding SCons warning message to mention mold linker
- Make PIE relocation detection glibc-only
- Alter linux debug stacktraces handling to support more environments
- Fix crash when calling `fill` method on an empty `Image`
- Fix .gitignore ignores part of the committed repo.
- zstd: Update to upstream version 1.5.5
- pcre2: Update to upstream version 10.42 (take two)
- miniupnpc: Update to version 2.2.5
- libpng: Update to upstream 1.6.40
- Ported:  Initialize GLWrapper earlier in Storage::initialize() Make sure `GLWrapper` is initialized before `glActiveTexture` is called by other parts of the storage initialize(), to prevent benign warnings. - lawnjelly https://github.com/godotengine/godot/commit/857d8840262b3339cda05dc77fe28de817322673
- Ported:  Unbind texture slots when changing framebuffer Prevent bugs whereby texture still in use. - lawnjelly https://github.com/godotengine/godot/commit/5e197fd21ea836a632d97e27a3dd5ecfaa8405f8
- Ported:  Hierarchical culling - Add extra check to skeleton_attach_canvas_item Although this check shouldn't be able to fail currently, it provides a small level of extra logic checking at only small cost in DEV builds. - lawnjelly https://github.com/godotengine/godot/commit/d7cca42ef6c720f455d1cc2c49c8667911f7ad78
- Ported:  Physics Interpolation 2D - reset on NOTIFICATION_ENTER_TREE As a convenience, physics interpolation is reset automatically on entering the tree. This will be desired in most situations, and saves the user having to write code for this explicitly. - lawnjelly https://github.com/godotengine/godot/commit/bcfca5ec8615cf796dcb02d4291d9475feb58031
- Ported:  Physics Interpolation - add support for CPUParticles2D Similar to the existing 3D CPUParticles physics interpolation. - lawnjelly https://github.com/godotengine/godot/commit/a117a3307a9eec69c5ed7c80a0b4eb272fafc474
- Ported:  Fix 2D MultiMesh hierarchical culling Fixes updating local bounds for MultiMeshes used in canvas items by introducing a back link. - lawnjelly https://github.com/godotengine/godot/commit/ad577e3c7e5e334144cbe15b46c670aaabb9b871
- Ported:  Add debug_canvas_item_get_local_bound() function to VisualServer Useful for debugging hierarchical culling. - lawnjelly https://github.com/godotengine/godot/commit/61e41cc9a1e22d0b15b8bd06ff848eed5d0f095f
- Ported:  2D Fixed Timestep Interpolation Adds support to canvas items and Camera2D. - lawnjelly https://github.com/godotengine/godot/commit/5162efbfe9daddb9b30c15c906890e0ea4e4213a
- Ported:  Physics interpolation - Move out of Scenario
- Remove unused variables.
- Fix non-tools builds.
- Ported:  SCons: Disable C++ exception handling
- Fix build.
- Make AnimationNodeBlendTree use OrderedHashMap insteads Map
- GLES2: Make GPU skinning more consistent
- Fix AnimatedSprite normal map loading
- Fix infinite loop on EOF in the command line debugger
- PulseAudio: Remove get_latency() caching
- Back-port notarytool, provisioning profile and PKG export options.
- Fix `ProjectSettings::localize_path` for Windows paths
- Backport AudioStream icons from 4.0
- Update npm packages
- Prevent double input events on gamepad when running through steam input #79706
- Update to upstream master.
- Explicitly handle buffer errors in send/recv
- MultiRect - Fix flushing in TextEdit
- Backport implement loading DDS textures at run-time

#### Godot4

- Backported the editor constrol changes for the new shortcut system from godot4.
- Backported the shortcut context system from godot4.
- Backported _shortcut_input() virtual from godot4.
- Backported the CallInputType parameter for SceneTree::_call_input_pause() from godot4.
- Backported the new output window from godot4.
- Backported SceneTree::get_first_node_in_group() from godot4.
- Backported from godot4:  Change GridMap navigation_layers to be per mesh_library item Changes GridMap navigation_layers from a single bitmask for the entire GridMap to a bitmask for each item used in the mesh_library with a baked navmesh. - smix8 https://github.com/godotengine/godot/commit/61f33e205cecc923786fb646f6e325657751a3f5
- Backported from godot4:  Add navigation tutorial links inside class doc Adds navigation tutorial links inside the class doc to the related and more detailed godot-docs pages. - smix8 https://github.com/godotengine/godot/commit/7506ecc5d96b6a894be05acca61593da73cccb81
- Backported from godot4:  Fix typo and ensure backwards compatibility for changed property names Changes to the name of the `navmesh` and `navpoly` properties on `NavigationRegion` caused navigation data to be lost on load. This PR creates uses `_set`/`_get` to handle compatibility with the older names on load, preserving the data. Also fixes a typo on `get_vertices_per_polygon` in `NavigationMesh`, and renames the property to remove the `polygon_` prefix which doesn't match the setter/getter. - DarkKilauea, akien-mga https://github.com/godotengine/godot/commit/0572346985eef45123a0f25cbd7c295e06bd9097
- Simplify some logic in NavigationAgent2D.
- Backported from godot4:  NavigationAgent2D, NavigationAgent3D typo fixes Fixed minor spelling errors in the NavigationAgent2D and NavigationAgent3D classes. - JustinDodemaide https://github.com/godotengine/godot/commit/29a228595c1b27b2755b0f5cd3465f05a7604c92
- Backported from godot4:  Fix NavigationAgent continues avoidance velocity Fixes NavigationAgent continues avoidance velocity. - smix8 https://github.com/godotengine/godot/commit/c912df9d0591a7abcab9d0eab5fac80c28117e9f
- Fix the tooltip of the Debug Paths option.
- Fix logic in NavMap::sync().
- Backported from godot4:  Remove SceneTree debug avoidance hint Removes SceneTree debug avoidance hint. - smix8 https://github.com/godotengine/godot/commit/f1d8ddd9bd03ff4debb5345ca302652b5a7858bc
- Backported from godot4: Fix NavigationObstacle2D debug position Fixes NavigationObstacle2D debug position. - smix8 https://github.com/godotengine/godot/commit/25121572947ff524651473bf2fdaaf411c9ef883
- Backported form godot4:  Add NavigationRegion function to change navigation map Adds NavigationRegion function to change navigation map. - smix8 https://github.com/smix8/godot/commit/84647ab09eba232ec7466dda9ce5fa269aad8bf3
- Backported from godot4:  Add NavigationServer API to enable regions and links Adds NavigationServer API to enable regions and links. - smix8 https://github.com/godotengine/godot/commit/69fad39cf5437b45bac2039d864605b2b63b9950
- Backported from godot4:  Update navigation obstacle API Updates navigation obstacle API. - smix8 https://github.com/godotengine/godot/commit/c1fc331b8865bd099df210fa434782bc9b60b2d8
- Backported from godot4:  Add agent pause mode to NavigationServer Adds agent pause mode to NavigationServer. - smix8 https://github.com/godotengine/godot/commit/ae9dd47d0c1c237d0733439862aa5ff651dcac2
- Backported from godot4:  Fix cell_height for navigation meshes Fixes `cell_height` for navigation meshes. - smix8 https://github.com/godotengine/godot/commit/180a5cded1d01e8c8965f009624652ee6ef1807c
- Backported from godot4:  Allow negative NavigationAgent2D path debug line_width for thin lines Allows the line_width for NavigationAgent2D path debug to go negativ for thin line rendering. - smix8 https://github.com/godotengine/godot/commit/f6a10c0565e32e0170bcce71635d8bad16077d1d
- Backported from godot4: Fix NavMesh map_update_id returning 0 results in errors - zorbathut https://github.com/godotengine/godot/commit/d0564f2466cec9d0e108cadf18f2aae642c90be7
- Backported from godot4:  Add detail to NavigationAgent signal descriptions Adds detail to NavigationAgent signal descriptions. - smix8 https://github.com/godotengine/godot/commit/99e70ab6efd18c0ed472041dcc2b7b3ae1c22de1
- Backported from godot4:  Fix pathfinding funnel adding unwanted point Fixes pathfinding funnel adding unwanted point due to precision issues. - smix8 https://github.com/godotengine/godot/commit/c51e2644466b96d414d4e42a9cfe283ce1162264
- Ported from godot4:  Fix NavigationAgent position not always updating Fixes NavigationAgent position not always updating. - smix8 https://github.com/godotengine/godot/commit/34bc410fb419cb002dbe6cd676f0ff54fab5db9c
- Ported from godot4:  Fix Navigation API abbreviations inconsistency Schema for navigation to name user facing API with  "navigation" without abbreviation and e.g. NavigationServer internals with abbr "nav". - smix8 https://github.com/godotengine/godot/commit/34e7628f5f1b419bfed1acd915209347e615bedf
- Add back the missing lines to GridMap's class doc.
- Backported from godot4:  Fix NavigationAgent3D debug path duplicated points Fixes duplicated points in NavigationAgent3D debug path. - smix8 https://github.com/godotengine/godot/commit/0b8798a9950ca7a86b1b2e20f69f22ba4396d55a
- Added in missing changes from  Add NavigationServer Performance Monitor Adds Performance Monitor for NavigationServer3D.. https://github.com/godotengine/godot/commit/9802914f9793b6888cc70e3d7f0d815bdd5188bb
- Backported from godot4:  Expose NavigationAgent path postprocessing and pathfinding algorithm options Exposes the path postprocessing and pathfinding algorithm options of the NavigationAgent internal NavigationPathQueryParameters object. - smix8 https://github.com/godotengine/godot/commit/6e324bb341c795905085e25e7f7c153d89949fa9
- Backported from godot4:  Tweak NavigationAgent3D defaults Tweaks default values for NavigationAgent3D to work better out of the box within a new 3D project. - smix8 https://github.com/godotengine/godot/commit/8be4af38e4a8a99a89cb258315080cf5f495e30f
- Backported from godot4:  Unbind NavigationServer3D.process() Unbinds NavigationServer3D.process(). - smix8 https://github.com/godotengine/godot/commit/f1026450bfe9bf4df2e059f220c2e579fa3e4f64
- Backported from godot4:  Improve NavigationServer.free() error msg when RID not found Improves the error msg when NavigationServer.free() does not find the RID e.g. because it was already deleted or did never exist in the first place. - smix8 https://github.com/godotengine/godot/commit/b51cab3411a18d0cce8f3bfd1e849a607ad4cab7
- Backported from godot4:  Tweak NavigationAgent2D defaults Tweaks default values for NavigationAgent2D to work better out of the box within a new 2D project using default resolution. - smix8 https://github.com/godotengine/godot/commit/594ffd220051d3be5b106f097ed8ca3371ab0ca0
- Backported from godot4:  Fix NavigationRegion3D debug mesh rendering twice in Editor Fixes double rendering of NavigationRegion3D debug mesh due to both 3DGizmo and runtime debug rendering the mesh at the same time. - smix8 https://github.com/godotengine/godot/commit/0e4c31ce57443d03a7d223618760998ad807def6
- Added missing doc for NavigationPolygon's cell_size.
- Backported from godot4:  Disable NavigationMesh edge_max_length property by default Disables NavigationMesh edge_max_length property by default. - smix8 https://github.com/godotengine/godot/commit/c63125c31760b4896414cf6e8bd67234c072f9a5
- Backported from godot4:  Add more hints to navigation map cell size errors Adds more hints to navigation map cell size errors. - smix8 https://github.com/godotengine/godot/commit/90e2d9fa7e4def8556919df58680746abe0ce5bf
- Backported from godot4:  Change 2D navigation ProjectSettings from integers to floats Changes 2D navigation ProjectSettings from integers to floats. - smix8 https://github.com/godotengine/godot/commit/2852fb43f707789bc05083302b1f2d6a19edec42
- Backported from godot4:  Fix closest possible navigation path position Fixes closest possible navigation path position. - smix8 https://github.com/godotengine/godot/commit/e5c24f7118854d36845af0de81d83da5ec18e2a8
- Backported from godot4: Fix crash in 'NavigationAgent3D', fixes #78910 - Scony https://github.com/godotengine/godot/commit/fcbb5213922829817b6ee76a15f1e4f7519b51d6
- Backported from godot4: Remove legacy navigation debug materials Removes legacy navigation debug materials. - smix8 https://github.com/godotengine/godot/commit/1b506cd49cdaa62d8cc8cdd3146007f3bcecf50f
- Backported from godot4:  Add NavigationRegion rotation warning Adds NavigationRegion rotation warning. - smix8 https://github.com/godotengine/godot/commit/4d8553ff6310c6f4d3c1dc4c9e2118dcd4570ae8
- Backported from godot4:  Add NavigationPolygon cell_size property - smix8 https://github.com/godotengine/godot/commit/9c8626bfd8b945787aa258c5706a6a2e0578e089
- Backported from godot4:  Improve navigation map synchronisation error msgs Improves navigation map synchronisation error msgs related to mismatch of cell sizes. - smix8 https://github.com/godotengine/godot/commit/7f2417135f75b1bb48d570db41e2127c9585238a
- Backported from godot4:  Add navigation map synchronization warnings. - smix8 https://github.com/godotengine/godot/commit/fef7b4efdc5653866a5fba3d2d6a515138c11a99
- Backported from godot4:  Fix --debug-avoidance description. Fixes c&p error in --debug-avoidance description. - smix8 https://github.com/godotengine/godot/commit/d171dfce9a5fcbe89ed5093484c85dab83fe3b88
- Backported from godot4:  Fix NavigationServer free error print Fixes error print for NavigationServer free when a RID can not be found. - smix8 https://github.com/godotengine/godot/commit/73dc680fc127014ad805e5968f98ebb3e0281de7
- Backported from godot4: Keep NavigationServer active while SceneTree is paused. - smix8 https://github.com/godotengine/godot/commit/4b0dee080e5eba37dd43346c6c02bb7dce2b39e7
- Backported from godot4: Strip name prefix of navigation agent debug properties in the inspector. - timothyqiu https://github.com/godotengine/godot/commit/65223e98de7504e61b414ff3858c5a2347acae56
- Backported from godot4: Stops NavigationAgents moving to the world origin without anyone telling them to do so. -smix8 https://github.com/godotengine/godot/commit/860379fc161698234b2757c6f62f8e05fa68ada8
- Backported from godot4:  Add GridMap function to change navigation map for baked navigation regions Adds function to change the navigation map for baked navigation regions. Before all cells with a baked navigation mesh were locked to the default navigation map of the world resource. - smix8 https://github.com/godotengine/godot/commit/41c529a94d7c690dca127bbc4f37c7f65d040f6b
- Backported from godot4:  Add Path2D/3D debug options - smix8 https://github.com/godotengine/godot/commit/e12e239ab4a9bb764105e3a0cde284f788cd59b3
- Ported from godot4: Fix reading Unicode from stdio. - bruvzg https://github.com/godotengine/godot/commit/3d8a942a56e1de32e23cd02eada3899c4d6d1033
- Pass in variables as intended to CreateProcessW().
- Ported from Godot4: Fix Windows execute exit code. - bruvzg https://github.com/godotengine/godot/commit/94355249c360be694bdb692f357dc017d742aee7

#### Other

- Ported godot pr: [3.x] Allow exporting custom resources from/to any scripting language (GDScript, VisualScript, C#, NativeScript, PluginScript) - willnationsdev https://github.com/godotengine/godot/pull/44879 Using the rebased version from https://github.com/Atlinx/godot/commit/02d1f70ee5ec1b7bb08bc6dddac7fe39b4727e1e by Atlinx

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

- Renamed Quaternion to Quaternion for consistency with the other engine math classes.
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
