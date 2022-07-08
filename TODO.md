# Pandemonium Engine TODOs

## WFC module

- Need to rework the readme. Also link to the sample repo. (https://github.com/Relintai/wfc_module_samples)
- TilingWaveFormCollapse should not generate the images themeslves, rather it should just use internal ids, and return those to you. It could store variants. -> a derived class chould be mamde that adds image generation on top, but in a friendlier way.
- Array2D and 3D's getters and setters that need coordinates use a reversed order compared to everything in the engine. This is super dangerous, and should be changed. (Currently: get(y, x). Should be get(x, y)).
- The classes need smaller fixes and touchups.
- There are probably a few lingering bugs, as some examples give bad results.
- Need to make sure wave size, out size, and size related things are consistent, and intuitive, as right now they are neither.
- The bindings will likely need more improvements.
- Would be nice, if the algorithm could be parametrized from gdscript aswell.
- Maybe addding Array2D, and Array3D like classes to the engine would be a good idea.

## Physics

- Add a multi thread wrapper around the physics server (and nav server).
- Add a method to the physics server that lets the caller thread wait for the physics update, and wait for each thread to finish.
- These 2 changes would enable heavy simplifications to most Props, Voxelman, and Terraman generation code, as they could just directly use the physics server in multi threaded jobs without crashing. The entire update mode thing could be removed.
- Add dummy physics servers, so they can be turned off when not needed. It should return rids that seem valid, so app code doesn't freak out anywhere.

## Navmeshes

- Add dummy nav server, so it can be turned off.

## Terraman, Voxelman, Props

- MaterialCache class (classes) should be either core or just an another module. (I think the build system has module dependencies implemented.)

## Terraman, Voxelman

- The rid storage API is using dicts currently, and it's pretty awful to mess with. A new mesh storage class should be added instead. Since more than one module needs it either should be core or a new module.  The api could just return a Ref<> to the storage class. And the class could be inherited from when needed. Should probably contain some generic api which allows you to store most things without really messing around too much. Like an api for mesh rids, navmesh rids, and collider rids, and generic rids.

## Editor

- Add support for enabling / disabling editor plugins, and also add a gui for this. (Like how the gdscript addons can be enabled/disabled, but for built in c++ stuff.) It will need support for either exlusion, or dependencies. (Dependencies would be better.)
- Modularize the editor. Should probably add a new editor_modules folder (Just normal modules, but they won't get compiled if tools are disabled).

## Engine

- Organize the core folder a bit better.
- (Maybe) add an SDL backend.

## Graphics 

- Refactor platform code so it's not automatically creating a window, instead the VS should ask for it's creation, passing along the requested graphics api.
- Add a dummy VisualServer implementation, and a way to switch between this and gles2. Similar to how godot4 can have a dummy renderer.

## Rendering

- Move CPUParticles code into the VisualServer.
- Add support for generating vertex colors from lights in the VisualServer. (Also do this for 2d)
- Add LOD support to the visualserver. (Or maybe the meshes) - Add core LOD generation support (probably VisualServer). (This and vertex color baking could drastically reduce the amount of code in Terraman, Voxelman, and terraman 2d, props)
- Maybe add a texture (mergedTexture) that makes it easier to handle texture merges.
- Add gles3 support, but just expand gles2 for it. (Like it could use better multimesh support, better particles etc), but it shouldn't be that different from gles2.
- Probably add support for other renderers, like vulkan, but unlike godot4 this should be realtively simple, with the visualserver design. (In the same veins as gles2/3)
- (And/or other highly portable graphics apis, that is if they exist.)
- Add a raw shader type that can be written in the native graphics api language (support per backend shader implementation for it). It will likely need some easily parsable header for shader properties. This might simplify the creation of other backends.
- Support for adding graphics backends from modules. This will require platform specific creation funcs.
- Could add an api to query gpus, and optionally to mess with them. Like you could use an off gpu to to do something, maybe like calculations, etc. (Needs more research, not even sure if this is possible or not, or how hard it would be.) 

## material_maker module

- Polygon editor needs fixes, and likely an overhaul.
- Curve editor needs smaller fixes, and likely an overhaul.
- Port the rest of the nodes from material maker.

## Databases

- Table builder should have a method that can automatically create tables from any Object using it's properties. Like tb->add_object_properties("obj type"). You could the still mess with the query, add fields, and set keys. It's purpose should be simple automation when you don't need handoptimized tables.
- Query builder should have more helper methods to automate simpler queries as much as possible. Like: sfw("col", "table", "a < 3") -> SELECT col FROM table WHERE a < 3;, or sf()... etc
- Query builder should also have an update and create, (and also get) method which generates a query using properties to automatically create sql to save objects. They could also take a pk argument to tell which entry is the key. save method would also be nice that decides whether to update or create an entry.
- No full ORM functionality though, it should be really close to SQL queries, but with lots of helper methods, for the more trivial stuff.
- Go through and double check that all methods for the QueryBuilder are properly set up, and is using escapes as they should.
- Fix postgres and mysql backends, so they can be built. (They should be off be default.)

## Web

- Test / fix HTML and BBCode parser, and make bindings for them.
- Move simple web server to a new module.
- Implement multi threading for the simple web server.
- Check how polling is implemented in sockets.
- Implement Cookie support for the web server.
- Implement file upload support.
- Implement proper mimetype support.
- Implement proper http header responses.
- Implement closing the connection only after answering the last query (by default, could be overrideable).

## Users

- Fix, and break UserController into smaller webnodes.
- Clean / flesh out the api of UserManager / UserDB. Also make UserManager register itself into UserDB.

