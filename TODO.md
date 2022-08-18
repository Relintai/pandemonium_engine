# Pandemonium Engine TODOs

## Core

- Add a core binder class for Math. This would allow for simpler script to engine side code ports. Maybe it could also smplify scripting language code.
- Maye threading could be de-modernized. So the implementation could use the old os level constructs. (like pthreads). An implementation that uses std::threads could be kept around though. (without it being compiled). 
- Maybe use NULLs again instead of nullptrs? TODO check if there is an actual difference, other than requiring cpp11.

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
- The current nav server uses worker threads (ThreadWorkPool), maybe it should use ThreadPool.
- The current nav server might not work if threading is disabled. TODO check.
- Maybe ThreadWorkPool could be removed.

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
- Reorganize the project. Except for core, and a few smaller things like main everything should be a module that can be disabled.
- Module hard dependencies would be a nice thing to have. For example if you turn off module A, and B has a hard dependency on it, B would turn itself off, or warn you.
- Add more hooks where needed, to let stuff register itself. Like visual server backends, so adding them is simple, as I think this is the best way forward, so you can just create your custom rendering backend, when needed. Like if you want lightmaps, add a module that has the tooling + visualserver that has it implemented.
- Port the DisplayServer from Godot4, as I think having display and windowing in a DisplayServer singleton instead of in OS is better architecturally.
- Port the capability to have more that one windows. Multi windowing however should be disabled by default, also I don't want to port the full multi windowing stuff for the editor, as it's a lot leaner the way it is. However multi windowing would be cool for creating tools. Would also make it easier to not have a window at all.
- The DisplayServer at first could just use stuff in OS, so it doesn't have to be ported all at onece.
- Should backport the tests and the test suite from Godot4 aswell.
- Backport optimizations to the hashmap (or maybe just add it as another option), and also replace usage where applicable (https://github.com/godotengine/godot/commit/8b7c7f5a753b43cec10f72b274bb1d70c253652b), and (https://github.com/godotengine/godot/commit/45af29da8095af16729955117a165d23e77cd740)
- Make operator='s return void (https://github.com/godotengine/godot/commit/7da392bcc52366740394322728464e724cf20cdf).
- Switch back to the old c++ version (pre c++11) for the codebase. I'm already targeting low end hardware, I think it will likely come in handy later.
- Implement something similar to this, so scripting language code can be simplified. (https://github.com/godotengine/godot/commit/5288ff538d75d2ddab257a9e1e40050c9b8fa1cb)(Create Variant built-in functions.) 
- Move DynamicFont to a module, and make tools depend on that.
- Maybe backport the TextServer from Godot4?
- Make modules from everything under the thisdparty folder.
- Collect license information from modules and merge it back and handle it like the current thirdparty.md file.
- Clean up the third party licenses.

## Scene folder

- Rework it into modules.

## GUI

- Rename the EditorZoomWidget to something like ZoomWidget, and make it available for the games aswell. Also I think I did smaller improvements to it in the world_generator addon. Also port those.

## Graphics 

- Refactor platform code so it's not automatically creating a window, instead the VS should ask for it's creation, passing along the requested graphics api.
- Add a dummy VisualServer implementation, and a way to switch between this and gles2. Similar to how godot4 can have a dummy renderer.

## Rendering

- Move CPUParticles code into the VisualServer.
- Add support for generating vertex colors from lights in the VisualServer. (Also do this for 2d)
- Add LOD support to the visualserver. (Or maybe the meshes) - Add core LOD generation support (probably VisualServer). (This and vertex color baking could drastically reduce the amount of code in Terraman, Voxelman, and terraman 2d, props) (https://github.com/godotengine/godot/commit/d2302f53d6ad80943e7f4245ac572003f1681d00)
- Maybe add a texture (mergedTexture) that makes it easier to handle texture merges.
- Add gles3 support, but just expand gles2 for it. (Like it could use better multimesh support, better particles etc), but it shouldn't be that different from gles2.
- Probably add support for other renderers, like vulkan, but unlike godot4 this should be realtively simple, with the visualserver design. (In the same veins as gles2/3)
- (And/or other highly portable graphics apis, that is if they exist.)
- Add a raw shader type that can be written in the native graphics api language (support per backend shader implementation for it). It will likely need some easily parsable header for shader properties. This might simplify the creation of other backends.
- Support for adding graphics backends from modules. This will require platform specific creation funcs.
- Could add an api to query gpus, and optionally to mess with them. Like you could use an off gpu to to do something, maybe like calculations, etc. (Needs more research, not even sure if this is possible or not, or how hard it would be.) 
- Rename VisualServer to RenderingServer like on Godot 4. I think it's a better long term name for what it does.
- Port the RenderingDevice API from Godot 4 alongside a RenderingServer implementation that uses it once it's stable. However keep the GLES backends around the same way as they are now, so RenderingServer would be Pluggable aswell: RenderingServer  <- RenderingSreverGLES2, RenderingServer  <- RenderingSreverGLES3 (eventually), RenderingServer  <- RenderingSreverRenderingDevice (This would then just use an available RenderingDevice).
- Port the newer godot rendering backends once they are stable enough. (With at least some of their features.)
- Somehow make VisualServer's implementation more modular.
- Add a scriptable VisualServer implementation. Similar idea as the scriptable networking api. Alongside with a similar interface for GLES. Would help with experimentation.

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

- Move simple web server to a new module.
- Implement multi threading for the simple web server.
- Implement big (or not in-memory) file upload support.
- Move HTMLParser, BBCodeParser, and MarkDownRenderer to a new (parsers?) module.
- BlogWebPage <- BlogWepBageFile, BlogWebPageDB
- NewsWebPage <- NewsWebPageFile, NewsWebPageDB
- AdminWebPage <- UserAdminWebPage, AdminPanelWebPage
- Add epoll api support to sockets, and make the web server use it.
- Add range http header support.
- Finish all the todos in the http parser.
- Fix unicode escape issues.
- Could add a simple templating language. Probably would not be more than a few hundred lines. WebPageTemplate (Resource), with an importer. Most webnodes could have assignable properties for them. Could be similar to laravel's for exmample. Also it could be pre compiled in an extremely simple fashion. Vector<Statement> StringStatement, IFStatement, LoopStatement, CallStatement etc. Oither than bool evaluation every statement type is extremely trivial. Also bool evaluation logic could be reused elsewhere (Also gdscript already have it implemented).

## Users

- Clean / flesh out the api of UserManager / UserDB. Also make UserManager register itself into UserDB.

## Ground Clutter

- Add new ground clutter module. It should have classes that help with spawning clutter like small objects, or foliage.
- Add GroundClutterData. It should contain all data that is needed to spawn in everything.
- Add GroundClutter Spatial node. Similar to TiledWalls, just adds in meshed.
- Add GroundClutterMerger Spatial node, which is the same as GroundClutter except it merges everything together.
- GroundClutterData should have a bool property, which causes spawners to raycast the ground and spawn stuff on hit.
- Should support foliage weights mode, which adds weights that shaders can use to add fake wind. (eventually)
- For Terraman/Voxelman they have to be registered and freed when chunks unload, also they should eventually handle chunk mesh generation events, if they need to raycast ground.
- When they need to raycast ground, they should wait until a raycast hits, maybe a few params could be added to how long they should try before giving up.
- Remove Ground clutter classes from terraman and voxelman.
- Should have prop support (by spawning in Nodes). It should not be merged.

## TiledWalls

- Finish implementing all collider types.
- Finish tiling types + z index support for other tiling that horizontal.
- Flavours + y_size probably has quite a few bugs.

## Ports

Currently the engine has everything that I wanted up to:
https://github.com/godotengine/godot/commit/5eaf83f2d71fbaf825f9ceb6a1246d6b87be3729
https://github.com/godotengine/godot/commit/5ea00ad3f05a367dfb3a076e48f241a03e973549

### Enable type information on release

Not sure whether to have this or not. It does increase binary size for release builds by quite a bit.

https://github.com/godotengine/godot/commit/226103d166e2806fdaee63242492233e810d6541
https://github.com/godotengine/godot/commit/663978e8ba5d8b3e37b9b1baf54a69e6a3a00656
https://github.com/godotengine/godot/commit/f0ca73892a26566e85e1fa819f151fa1db4a42ad
https://github.com/godotengine/godot/commit/3a8f2dda01adca0086a79347484d01b483c8b624


### Physics interpolation - Move out of Scenario 

- Move VisualServer interpolation data out of Scenario and into VisualServerScene, so the interpolation data and enabled status is now common to all Scenarios.
- Fix physics interpolation in multithreaded mode by ensuring tick and pre-draw are called.

At first glance this might have the side effect of not being able to run multiple physics worlds. If this is true this is not needed.

The fix might be needed though. Double check.

https://github.com/godotengine/godot/commit/d2b1d296340bb514b16af037c7ad6ed19e7b4a3f

###  Fix the issue causing the screen to be black after resuming when in low processor mode.

I did the low processor mode fix differently than in upstream godot, so need to check.

If needed then it's probably only the Main::force_redraw() call.

https://github.com/godotengine/godot/commit/7a88d5bf283952cfcccef111143c88923017599e

### Physics Interpolation - add helper warnings 

Not sure if it's needed.

https://github.com/godotengine/godot/commit/ad9b2b3794f33429182923f55d3451dc98063617
https://github.com/godotengine/godot/commit/f1bef3c592ef18e0877fff913b5c1a8c1bfc9121

### Remove duplicate editor settings definitions 

Has to be hand ported, however some of my modules will likely need a similar cleanup, so todo.

https://github.com/godotengine/godot/commit/d088128b439d2fddf08f0b87090d756dd417b6e1

### Display a modal dialog if threads are unavailable in the web editor 

Not sure, if I end up porting it, I'll likely simplify it.

https://github.com/godotengine/godot/commit/d9acd21b46352a0c6fe09b42d03bbdb6a749687d


