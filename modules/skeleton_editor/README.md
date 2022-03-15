# Skeleton Editor

This is a c++ engine module for the Godot engine that contains a modularized version of TokageItLab's pr's 3.2 version from the godot engine repository, until it gets merged.

The original pr is here: https://github.com/godotengine/godot/pull/45699
Tht 3.x version (linked in the pr itself) is here  (This is the base for this module): https://github.com/TokageItLab/godot/tree/pose-edit-mode 

I'm developing this for godot 3.x, it will probably work on earlier versions though. 4.0 is not supported.

# Building

1. Get the source code for the engine.

```git clone -b 3.x https://github.com/godotengine/godot.git godot```

2. Go into Godot's modules directory.

```
cd ./godot/modules/
```

3. Clone this repository

```
git clone https://github.com/Relintai/skeleton_editor skeleton_editor
```

4. Build Godot. [Tutorial](https://docs.godotengine.org/en/latest/development/compiling/index.html)
