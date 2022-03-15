# Texture Packer for the Godot Engine

This is a texture packer engine module, for the Godot Engine.

It can create texture atlases for you even in the running game.

It uses the legacy version of [rectpack2D](https://github.com/TeamHypersomnia/rectpack2D/tree/legacy)

It should work on all platforms.

It supports both godot 3.2 and 4.0 (master [last tested commit](https://github.com/godotengine/godot/commit/b7e10141197fdd9b0dbc4cfa7890329510d36540)). Note that since 4.0 is still in very early stages I only 
check whether it works from time to time.

# Pre-built binaries

You can grab a pre-built editor binary from the [Broken Seals](https://github.com/Relintai/broken_seals/releases) 
repo, should you want to. It contains all my modules.

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
git clone https://github.com/Relintai/texture_packer texture_packer
```

4. Build Godot. [Tutorial](https://docs.godotengine.org/en/latest/development/compiling/index.html)

# Features and Usage

## TexturePacker

This is the class that can merge textures. Add every texture you want into it using it's API (add_texture()), and then call merge().

add_texture() will return an AtlasTexture, this is the texture you want to use in your classes. It is immediately usable, it will just contain the original texture. Calling merge() will change it, to point to the new (merged) texture.

Supports filters, custom background color, margins.

### The keep_original_atlases option:

If you set this to true, and then add AtlasTextures, TexturePacker will change these ones (the ones you actually added) 
after the bake.

You can use this to bake gui textures together, without changing the resources everywhere at runtime.
Think of rpgs, when you have a huge number of potential icons that the player can put on his or her actionbars.
You can take look at Tales of Maj'Eyal or pretty much every actually complex MMORPGs as an example.

Note: Doing something like this in only recommended, if you can't pre-make the atlases (or it's really unfeasible), you are better off 
making the atlases yourself during development.

## TextureMerger

A Node that can bake textures for you. It uses TexturePacker internally.

It has an exposed Array, so you can assign textures to it in the editor.

## PackerImageResource

This is a simple Texture, which just contains an imported Image. It has no logic for drawing.

Useful for textures you only need for baking, as this class will not register it's data into the VisualServer.

The module also contains an editor plugin which can import textures as `PackerImageResource` Resource.

To access it, click on a texture, switch to the import tab, and in the "Import As" Dropdown, select "Packer Image Recource".

## TextureLayerMerger

This class can merge together textures as layers. Useful for example to merge together (and color) skin, and clothes for a character.

It can handle both AtlasTextures and normal Textures.

Add the layers from bottom to top with the add_texture() method, when you added everything call merge().

You can set the resulting image's size with the `width`, and `height` properties. If you leave them at 0, they will
change to the first added texture's size.

add_texture looks like this:

``` 
void add_texture(Ref<Texture> p_texture, Color p_color = Color(1, 1, 1, 1), Vector2 p_position = Vector2(), Rect2 p_rect = Rect2());
```

The color parameter will color the given texture on merge().
With the position parameter you can offset your texture (in the resulted texture), and with the rect parameter, you can crop it.

There are setters to manipulate the added data later.

After the merge, you can either use `get_result_as_texture()` (it creates an ImageTexture on the fly), or the `data` property to 
grab the resulting Image.
