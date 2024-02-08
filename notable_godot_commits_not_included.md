# Notable Godot Commits that are not included

These are some of the commits that I decided against having by default, but they might be of interest for other projects.

## Use circular fade instead of linear fade for distance fade

This makes distance fade look the same regardless of the camera angle,
for all distance fade modes (Pixel Alpha, Pixel Dither, Object Dither).
Distance fade now behaves like fog in this regard.

https://github.com/godotengine/godot/commit/56ac14fbf0240287ca93b625ba55dec6217b244f

## [3.x] Backport DirectionalLight fade_start property

Not sure yet

https://github.com/godotengine/godot/commit/dc776e46b65bce8508e393b5a7d7fe358e3dbedb

https://github.com/godotengine/godot/commit/4fefb136ea66633c20698e24d75b3fcdd4ca4a96
