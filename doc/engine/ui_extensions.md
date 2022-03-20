# UI Extensions

This is a c++ engine module for the Godot engine, containing smaller utilities.

It supports both godot 3.2 and 4.0 (master [last tested commit](https://github.com/godotengine/godot/commit/b7e10141197fdd9b0dbc4cfa7890329510d36540)). Note that since 4.0 is still in very early stages I only
check whether it works from time to time.

# Pre-built binaries

You can grab a pre-built editor binary from the [Broken Seals](https://github.com/Relintai/broken_seals/releases)
repo, should you want to. It contains all my modules.

# TouchButton

A `Control` based button, that handles multitouch properly.

# BSInputEventKey

An `inputEventKey` implementation, that matches actions exactly.

For example with the default godot implementation if you have an action that gets triggered
with the key `E` then `Ctrl-E` will also trigger it.

This has the side effect, that if you bind an action to `E`, and an another one to `Ctrl-E`,
then hitting `Ctrl-E` will trigger both.

This implementation changes that behaviour.

However, you do need to replace normal input events at startup like this:

```
func _ready():
	var actions : Array = InputMap.get_actions()

	for action in actions:
		var acts : Array = InputMap.get_action_list(action)

		for i in range(len(acts)):
			var a = acts[i]
			if a is InputEventKey:
				var nie : BSInputEventKey = BSInputEventKey.new()
				nie.from_input_event_key(a as InputEventKey)
				acts[i] = nie

				InputMap.action_erase_event(action, a)
				InputMap.action_add_event(action, nie)

```

I recommend putting this code into a singleton.

# Building

1. Get the source code for the engine.

If you want Godot 3.2:
```git clone -b 3.2 https://github.com/godotengine/godot.git godot```

If you want Godot 4.0:
```git clone https://github.com/godotengine/godot.git godot```


2. Go into Godot's modules directory.

```
cd ./godot/modules/
```

3. Clone this repository

```
git clone https://github.com/Relintai/ui_extensions ui_extensions
```

4. Build Godot. [Tutorial](https://docs.godotengine.org/en/latest/development/compiling/index.html)



