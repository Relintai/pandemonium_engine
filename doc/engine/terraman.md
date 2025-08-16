# Terraman

A terrain engine for godot, focusing more on editor integration, gameplay-related features, and extendability (even from gdscript), without sacrificing too much speed.

It is a spinoff of [Voxelman](https://github.com/Relintai/voxelman). I started working on it when I realized that not only a full 3d voxel engine is too hard for me to use properly for an rpg (just think about how hard it is to do smooth zone - zone and dungeon transitions with the proper fidelity for an rpg), it's also unnecessary.

I could have technically implemented all of this into voxelman, as having only have one row of chunks, and then setting chunk height to 1, and creating a mesher that reads isolevel values as a normal height map will achieve the same effect. However as voxelman has lots of features with noises, lights and vertices, adding this on top of that module would have ended up being messy just for this reason alone (and also let's not forget the 3d apis).

So I ended up creating this. Everything works the same as in voxelman, but the apis have been simplified to make UX a bit better.

This is an engine module! Which means that you will need to compile it into Godot! [See the compiling section here.](#compiling)

You can grab pre-built binaries (even editor + export templates) from the [Broken Seals](https://github.com/Relintai/broken_seals/releases) repo.

## Godot Version Support

3.2 - Will likely work, probably needs changes by now. (TODO check.)\
3.3 - Will more likely work, might need smaller changes by now. (TODO check.)\
3.4 - Should work without any issues. (TODO check.)\
3.x - Works.\
4.0 - Have been fixing support from time to time. Currently it won't build. Mostly done with the fix though.

## Optional Dependencies

`https://github.com/Relintai/texture_packer`: You get access to [TerraLibraryMerger](#voxellibrarymerger) and [TerraLibraryMergerPCM](#voxellibrarymergerpcm). \
`https://github.com/Relintai/mesh_data_resource`: You get access to a bunch of properties, and methods that can manipulate meshes.\
`https://github.com/Relintai/props`: You get access to a bunch of properties, and methods that can manipulate, and use props.\
`https://github.com/Relintai/mesh_utils`: Lets you use lod levels higher than 4 by default.

## Usage

First create a scene, and add a TerraWorldBlocky node into it. Create a TerraLibrary, and assign it to the Library property.
Also, add a TerraSurface into your library.

Tick the editable property, deselect, then select the world again, and click the insert button at the top toolbar, or press B to insert a
voxel at the inspector's camera's location.

Select the add button, and now you can just add voxels with the mouse, by clicking on the newly added voxel.

## TerraLibrary

This class stores the materials, and the TerraSurfaces.

Lod levels will automatically try to use materials of their own index.\
For example lod level 1 will try to use material index 1, lod level 2 will try to use material index 2, etc.\
If a material index is not available, they'll use the highest that is.\
For example lod level 5 will try to get material index 5, but if you only have 3 materials it will use the 3rd.

### TerraLibrarySimple

The simplest library, just assign a material with a texture, and using the atlas_rows and atlas_culomns properties to tell the system
how the UVs should be divided.

This is the basic Minecraft-style lib rary. Use this if you just have one texture atlas.

### TerraLibraryMerger

You will only have this if your godot also contains https://github.com/Relintai/texture_packer

You can assign any texture to your surfaces with this, and it will merge them together.

### TerraLibraryMergerPCM

(PCM = Per Chunk Material)

You will only have this if your godot also contains https://github.com/Relintai/texture_packer

You can assign any texture to your surfaces with this, and it will merge them together, but it will do it for every required chunk/voxel combination.

For example if you have a chunk with voxel Grass, and voxel Stone used in it, this library will create a material with a merged texture for Stone and Grass.
If you have an anouther chunk which only has Grass and Stone in it, this material will be reused.
And if you have a third chunk which only has a Grass voxel used in it, it will get a new merged material and texture only containing Grass voxel.

## Worlds

The 2 base classes. These won't do meshing on their own:

TerraWorld: Basic world, does not do anything until you implemnent the required virtual methods!\
TerraWorldDefault: This adds threading, and LoD storage support to TerraWorld. Will not create meshes for you!

### TerraWorldBlocky

It generated UV mapped standard simple terrain meshes.
The default algorithm can also generate normal lods.

### Level generation

Assign a TerraManLevelGenerator to the `World`'s `Level Generator` property.

You can write your own algorithm by implementing the ``` void _generate_chunk(chunk: TerraChunk) virtual ``` method.

`TerraManLevelGeneratorFlat` is also available, it will generate a floor for you, if you use it.

## TerraJobs

Producing just a terrain mesh for a chunk is not that hard by itself. However when you start adding layers/features
like lod generation, collision meshes (especially since manipulating the physics server is not threadsafe),
vertex lights, props, snapping props, props with vertex lights, etc
chunk mesh generation can quickly become a serious mess.

TerraJobs are meant to solve the issue with less complexity.

They also provide a way to easily modularize mesh and lod generation.

### TerraJob

Base class for jobs.

This is inherited from `ThreadPoolJob`.

A job has a reference to it's owner chunk.

If you implement your own jobs, when your job finishes call `next_job()`.

### TerraLightJob

This is the job that will generate vertex light based ao, random ao, and will bake your `TerraLight`s.

### TerraTerrainJob

This will generate your terrain collider and mesh (with lods) for you, using the meshers that you add into it.

Your lod setup is easily customizable with [TerraMesherJobSteps](https://github.com/Relintai/terraman/blob/master/world/jobs/voxel_mesher_job_step.h). The setup happens in your selected world's `_create_chunk` method.

### TerraPropJob

This will generate your prop meshes (with lods).

Also supports [TerraMesherJobSteps](https://github.com/Relintai/terraman/blob/master/world/jobs/voxel_mesher_job_step.h).

### Internal workings

#### TerraWorld

Whenever you want to spawn a chunk your World will create it using the ``` TerraChunk _create_chunk(x: int, y: int, z: int, chunk: TerraChunk) virtual ``` method.

Since properly initializing a chunk usually takes quite a few steps that you probably don't want to repeat everywhere the `chunk`
parameter was added. This means you can just call the super `_create_chunk` methods, and you won't need to worry about your chunk
getting overridden. Like:

Note that `_create_chunk` is also responsible for initializing chunks if you have them stored inside a scene.
This is done by `setup_chunk(shunk)` in `TerraWorld`.

```
    func _create_chunk(x : int, y : int, z : int, chunk : TerraChunk) -> TerraChunk:
        if !chunk:
            chunk = MyChunk.new()

        # We need to check whether or not we need to initialize jobs
        if chunk.job_get_count() == 0:
            # Setup a blocky (minecratf like) mesher job
            var tj : TerraTerrainJob = TerraTerrainJob.new()

            var s : TerraMesherJobStep = TerraMesherJobStep.new()
            s.job_type = TerraMesherJobStep.TYPE_NORMAL
            tj.add_job_step(s)

            tj.add_mesher(TerraMesherBlocky.new())
            tj.add_liquid_mesher(TerraMesherLiquidBlocky.new())

            chunk.job_add(tj);

        #setup your chunk here

        return ._create_chunk(x, y, z, chunk)
```

You can look at the world implementations for more examples: [TerraWorldBlocky](https://github.com/Relintai/terraman/blob/master/world/blocky/voxel_world_blocky.cpp).

#### TerraChunk

Stores terrain data, prop data. And mesh data (TerraChunkDefault), and the mesh generation jobs.

When it starts building meshes it will start submitting jobs to thread_pool (if present) one by one.

#### TerraMesher

If you want to implement your own meshing algorithm you can do so by overriding ``` void _add_chunk(chunk: TerraChunk) virtual ```.

TerraMesher works similarly to SurfaceTool, so first you need to set colors, uvs, etc and then call add_vertex.
They won't get reset, so for example if you want all your vertices to have a certain color, you can get away with setting it only once.

## Compiling

First make sure that you can compile godot. See the official docs: https://docs.godotengine.org/en/3.x/development/compiling/index.html

1. Clone the engine if you haven't already:

If you want Godot 3.x:
```git clone -b 3.x https://github.com/godotengine/godot.git godot```

If you want Godot 4.0:
```git clone https://github.com/godotengine/godot.git godot```


2. go into the modules folder inside the engine's directory:

```cd godot``` \
```cd modules```

3. clone this repository

```git clone https://github.com/Relintai/terraman.git terraman```

(the folder needs to be named terraman!)

4. If you want the optional dependencies run these commands as well:

```git clone https://github.com/Relintai/texture_packer.git texture_packer``` \
```git clone https://github.com/Relintai/mesh_data_resource.git mesh_data_resource```

5. Go up one folder

```cd ..```

6. Compile godot.

For example:

```scons p=x11 t=release_debug tools=yes```
