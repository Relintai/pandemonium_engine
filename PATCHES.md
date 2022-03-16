Applied / backported patches / prs
==================================

Skeleton Editor
---------------

The engine contains TokageItLab's SkeletonEditor prs 3.2 version from the godot engine repository.

Original pr: https://github.com/godotengine/godot/pull/45699
3.2 version: https://github.com/TokageItLab/godot/commits/pose-edit-mode

In order to simplify merging upstream godot commits I decided to port the necessary engine side changes by hand, (hopefully) in a way which minimalizes merge conflicts later. The editor plugins themselves are available via an engine module (modules/skeleton_editor).

Core Changing Commits:
cb6f4aea984ab782608b91a7c5be215ab262e349
66a93366886b46e0b7fab4e3b54e0b59737c8d19


Patches that I'm considering
============================

- https://github.com/godotengine/godot/pull/52391 - Added Viewport canvas cull mask / CanvasItem rendering layers.
- https://github.com/godotengine/godot/pull/51840 - Add an option to preview the 3D scene as a background to the 2D editor
- https://github.com/godotengine/godot/pull/50718 - Drag multiple resources onto exported array variable at once
- https://github.com/godotengine/godot/pull/48965 - Tell rpc target method/property when node is not found
- https://github.com/godotengine/godot/pull/47170 - print_stray_nodes() also prints node's script
- https://github.com/godotengine/godot/pull/44420 - Added ability to have Tabs in TabContainer at bottom
- https://github.com/godotengine/godot/pull/42681 - (Either) Improve performance when apply bone transform in get_bone_global_pose()
- https://github.com/godotengine/godot/pull/41322 - (Or) PhysicsBone3D and Skeleton Improvements
- https://github.com/godotengine/godot/pull/42642 - Ensure KinematicBodies only interact with other Bodies with layers in their mask
- https://github.com/godotengine/godot/pull/42365 - Increment index when populating PhysicsShapeQueryParameters exclude array.
- https://github.com/godotengine/godot/pull/41699 - Fix Area monitorable in 2D and 3D Godot physics.
- https://github.com/godotengine/godot/pull/40981 - Always disable low-processor usage mode on mobile platforms - (Or fix it)
- https://github.com/godotengine/godot/pull/39362 - Fix PlaneShape in Godot physics
- https://github.com/godotengine/godot/pull/37680 - Editor Debugger: auto scrolling is activated at every log start.

- https://github.com/godotengine/godot/pull/36776 - [WIP] Editor Android Port
-- 3.x version - https://github.com/thebestnom/godot/commits/android_editor_3.2

- The full 4.0 TileMap + TileMap Editor rework. (Like the skeleton editor it would become a module)

Maybe, Probably later

- https://github.com/godotengine/godot/pull/37200 - NetworkedController & SceneRewinder

Not as important, but would be cool

- https://github.com/godotengine/godot/pull/50622 - Add highlight to the relationship lines of selected Tree items
- https://github.com/godotengine/godot/pull/48395 - Backport nav server to 3.x
- https://github.com/godotengine/godot/pull/50156 - Some fixes to runtime navmesh baking (prev pr might need it)

Might get merged soon, not sure

- https://github.com/godotengine/godot/pull/37765 - control node global rect scale fix

Not sure

- Backport polygon2D Editor + https://github.com/godotengine/godot/pull/40152 - Enable addition of points directly in Polygon2D UV Editor
- https://github.com/godotengine/godot/pull/44845 - Add optional ANGLE support for Windows desktop.

These will probably get merged eventually on upstream, it might be worth considering them again later

- https://github.com/godotengine/godot/pull/52793 - added ninja tool for scons
- https://github.com/godotengine/godot/pull/52912 - added scons ccache tool
- https://github.com/godotengine/godot/pull/52566 - Added support for building shared libraries on linuxbsd
