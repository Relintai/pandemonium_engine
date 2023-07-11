"""Functions used to generate scu build source files during build time
"""
import glob, os
import math
from pathlib import Path
from os.path import normpath, basename

base_folder_path = str(Path(__file__).parent) + "/"
base_folder_only = os.path.basename(os.path.normpath(base_folder_path))
_verbose = False
_is_release_build = False
_scu_folders = set()


def clear_out_existing_files(output_folder, extension):
    output_folder = os.path.abspath(output_folder)
    # print("clear_out_existing_files from folder: " + output_folder)

    if not os.path.isdir(output_folder):
        # folder does not exist or has not been created yet,
        # no files to clearout. (this is not an error)
        return

    os.chdir(output_folder)

    for file in glob.glob("*." + extension):
        # print("removed pre-existing file: " + file)
        os.remove(file)


def folder_not_found(folder):
    abs_folder = base_folder_path + folder + "/"
    return not os.path.isdir(abs_folder)


def find_files_in_folder(folder, sub_folder, include_list, extension, sought_exceptions, found_exceptions):
    abs_folder = base_folder_path + folder + "/" + sub_folder

    if not os.path.isdir(abs_folder):
        print("ERROR " + abs_folder + " not found.")
        return include_list, found_exceptions

    os.chdir(abs_folder)

    sub_folder_slashed = ""
    if sub_folder != "":
        sub_folder_slashed = sub_folder + "/"

    for file in glob.glob("*." + extension):

        simple_name = Path(file).stem

        if file.endswith(".gen.cpp"):
            continue

        li = '#include "' + folder + "/" + sub_folder_slashed + file + '"'

        if not simple_name in sought_exceptions:
            include_list.append(li)
        else:
            found_exceptions.append(li)

    return include_list, found_exceptions


def write_output_file(file_count, include_list, start_line, end_line, output_folder, output_filename_prefix, extension):

    output_folder = os.path.abspath(output_folder)

    if not os.path.isdir(output_folder):
        # create
        os.mkdir(output_folder)
        if not os.path.isdir(output_folder):
            print("ERROR " + output_folder + " could not be created.")
            return
        print("CREATING folder " + output_folder)

    file_text = ""

    for l in range(start_line, end_line):
        if l < len(include_list):
            line = include_list[l]
            li = line + "\n"
            file_text += li

    num_string = ""
    if file_count > 0:
        num_string = "_" + str(file_count)

    short_filename = output_filename_prefix + num_string + ".gen." + extension
    output_filename = output_folder + "/" + short_filename
    if _verbose:
        print("generating: " + short_filename)

    output_path = Path(output_filename)
    output_path.write_text(file_text, encoding="utf8")


def write_exception_output_file(file_count, exception_string, output_folder, output_filename_prefix, extension):
    output_folder = os.path.abspath(output_folder)
    if not os.path.isdir(output_folder):
        print("ERROR " + output_folder + " does not exist.")
        return

    file_text = exception_string + "\n"

    num_string = ""
    if file_count > 0:
        num_string = "_" + str(file_count)

    short_filename = output_filename_prefix + "_exception" + num_string + ".gen." + extension
    output_filename = output_folder + "/" + short_filename

    if _verbose:
        print("generating: " + short_filename)

    output_path = Path(output_filename)
    output_path.write_text(file_text, encoding="utf8")


def find_section_name(sub_folder):
    # Construct a useful name for the section from the path for debug logging
    section_path = os.path.abspath(base_folder_path + sub_folder) + "/"

    folders = []
    folder = ""

    for i in range(8):
        folder = os.path.dirname(section_path)
        folder = os.path.basename(folder)
        if folder == base_folder_only:
            break
        folders.append(folder)
        section_path += "../"
        section_path = os.path.abspath(section_path) + "/"

    section_name = ""
    for n in range(len(folders)):
        section_name += folders[len(folders) - n - 1]
        if n != (len(folders) - 1):
            section_name += "_"

    return section_name


# "folders" is a list of folders to add all the files from to add to the SCU
# "section (like a module)". The name of the scu file will be derived from the first folder
# (thus e.g. scene/3d becomes scu_scene_3d.gen.cpp)

# "includes_per_scu" limits the number of includes in a single scu file.
# This allows the module to be built in several translation units instead of just 1.
# This will usually be slower to compile but will use less memory per compiler instance, which
# is most relevant in release builds.

# "sought_exceptions" are a list of files (without extension) that contain
# e.g. naming conflicts, and are therefore not suitable for the scu build.
# These will automatically be placed in their own separate scu file,
# which is slow like a normal build, but prevents the naming conflicts.
# Ideally in these situations, the source code should be changed to prevent naming conflicts.

# "extension" will usually be cpp, but can also be set to c (for e.g. third party libraries that use c)
def process_folder(folders, sought_exceptions=[], includes_per_scu=0, extension="cpp"):
    if len(folders) == 0:
        return

    # Construct the filename prefix from the FIRST folder name
    # e.g. "scene_3d"
    out_filename = find_section_name(folders[0])

    found_includes = []
    found_exceptions = []

    main_folder = folders[0]
    abs_main_folder = base_folder_path + main_folder

    # Keep a record of all folders that have been processed for SCU,
    # this enables deciding what to do when we call "add_source_files()"
    global _scu_folders
    _scu_folders.add(main_folder)

    # main folder (first)
    found_includes, found_exceptions = find_files_in_folder(
        main_folder, "", found_includes, extension, sought_exceptions, found_exceptions
    )

    # sub folders
    for d in range(1, len(folders)):
        found_includes, found_exceptions = find_files_in_folder(
            main_folder, folders[d], found_includes, extension, sought_exceptions, found_exceptions
        )

    found_includes = sorted(found_includes)

    # calculate how many lines to write in each file
    total_lines = len(found_includes)

    # adjust number of output files according to whether DEV or release
    num_output_files = 1
    if _is_release_build:
        # always have a maximum in release
        includes_per_scu = 8
        num_output_files = max(math.ceil(total_lines / includes_per_scu), 1)
    else:
        if includes_per_scu > 0:
            num_output_files = max(math.ceil(total_lines / includes_per_scu), 1)

    # error condition
    if total_lines == 0:
        return

    lines_per_file = math.ceil(total_lines / num_output_files)
    lines_per_file = max(lines_per_file, 1)

    start_line = 0
    file_number = 0

    # These do not vary throughout the loop
    output_folder = abs_main_folder + "/.scu/"
    output_filename_prefix = "scu_" + out_filename

    # Clear out any existing files (usually we will be overwriting,
    # but we want to remove any that are pre-existing that will not be
    # overwritten, so as to not compile anything stale)
    clear_out_existing_files(output_folder, extension)

    for file_count in range(0, num_output_files):
        end_line = start_line + lines_per_file

        # special case to cover rounding error in final file
        if file_count == (num_output_files - 1):
            end_line = len(found_includes)

        write_output_file(
            file_count, found_includes, start_line, end_line, output_folder, output_filename_prefix, extension
        )

        start_line = end_line

    # Write the exceptions each in their own scu gen file,
    # so they can effectively compile in "old style / normal build".
    for exception_count in range(len(found_exceptions)):
        write_exception_output_file(
            exception_count, found_exceptions[exception_count], output_folder, output_filename_prefix, extension
        )


def generate_scu_files(verbose, is_release_build):

    print("=============================")
    print("Single Compilation Unit Build")
    print("=============================")
    print("Generating SCU build files")
    global _verbose
    _verbose = verbose
    global _is_release_build
    _is_release_build = is_release_build

    curr_folder = os.path.abspath("./")

    # check we are running from the correct folder
    if folder_not_found("core") or folder_not_found("platform") or folder_not_found("scene"):
        raise RuntimeError("scu_builders.py must be run from the godot folder.")
        return

    process_folder(["core"])
    process_folder(["core/bind"])
    process_folder(["core/config"])
    process_folder(["core/containers"])
    process_folder(["core/error"])
    process_folder(["core/input"])
    process_folder(["core/log"])
    process_folder(["core/math"])
    process_folder(["core/object"])
    process_folder(["core/os"])
    process_folder(["core/string"])
    process_folder(["core/variant"])
    process_folder(["core/io"])
    process_folder(["core/crypto"])

    process_folder(["drivers/gles2"])
    process_folder(["drivers/unix"])
    process_folder(["drivers/png"])

    process_folder(["editor"])
    process_folder(["editor/import"])
    process_folder(["editor/plugins"])

    process_folder(["main"])
    process_folder(["main/tests"])

    process_folder(
        [
            "platform",
            "android/export",
            "iphone/export",
            "javascript/export",
            "osx/export",
            #"uwp/export",
            "windows/export",
            "x11/export",
        ]
    )

    #TODO These should be moved to module conpig.py s

    process_folder(["modules/broken_seals_module"])
    process_folder(["modules/cscript"])
    process_folder(["modules/csg"])

    process_folder(["modules/database"])
    process_folder(["modules/database_sqlite"])

    process_folder(["modules/entity_spell_system",
        "data/atlases",
        "data/auras",
        "data/items",
        "data/loot",
        "data/species",
        "data/spells",
        "database",
        "drag_and_drop",
        "editor",
        "entities",
        "entities/ai",
        "entities/auras",
        "entities/data",
        "entities/resources",
        "entities/skills",
        "entities/stats",
        "formations",
        "infos",
        "inventory",
        "material_cache",
        "pipelines",
        "profiles",
        "profiles/actionbar",
        "profiles/input",
        "projectiles/3d",
        "props",
        "singletons",
        "skeleton",
        "spawners",
        "utility",
    ])

    process_folder(["modules/fastnoise"])

    process_folder(["modules/gdnative",
        "gdnative",
        "nativescript",
        #"arvr",
        "pluginscript",
        "net",
    ])

    process_folder(["modules/gdscript"])
    #process_folder(["modules/gdscript/language_server"])

    process_folder(["modules/gridmap"])
    process_folder(["modules/gridmap/geometry_parser"])

    process_folder(["modules/http_server_simple"])

    process_folder(["modules/lz4"])

    process_folder(["modules/material_maker",
        "algos",
        "editor",
        "editor/widgets/color_picker_popup",
        "editor/widgets/curve_edit",
        "editor/widgets/file_dialog",
        "editor/widgets/float_edit",
        "editor/widgets/gradient_editor",
        "editor/widgets/image_picker_button",
        "editor/widgets/mm_dnd_color_picker_button",
        "editor/widgets/polygon_edit",
        "editor/widgets/tones_editor",
        "nodes",
        "nodes/bases",
        "nodes/filter",
        "nodes/gradient",
        "nodes/noise",
        "nodes/other",
        "nodes/pattern",
        "nodes/sdf2d",
        "nodes/sdf3d",
        "nodes/simple",
        "nodes/transform",
        "nodes/uniform",
    ])

    process_folder(["modules/mbedtls"])

    process_folder(["modules/mesh_data_resource",
        "editor",
        "editor/utilities",
        "editor/uv_editor",
        "nodes",
        "plugin",
        "plugin_gltf",
        "props",
        "props_2d",
    ])

    process_folder(["modules/mesh_utils"])

    process_folder(["modules/navigation"])
    process_folder(["modules/navigation_dummy"])
    process_folder(["modules/navigation_geometry_parsers",
        "geometry_parser_2d",
        "geometry_parser_3d",
    ])
    process_folder(["modules/navigation_mesh_generator"])
    process_folder(["modules/navigation_mesh_generator/editor"])

    process_folder(["modules/network_synchronizer"])

    process_folder(["modules/paint",
        "actions",
        "editor",
        "nodes",
        "ui",
        "ui/property_inspectors",
    ])

    process_folder(["modules/props",
        "clutter",
        "editor",
        "jobs",
        "lights",
        "material_cache",
        "props",
        "singleton",
        "tiled_wall",
    ])

    process_folder(["modules/props_2d",
        "clutter",
        "editor",
        "jobs",
        "lights",
        "material_cache",
        "props",
        "singleton",
        "tiled_wall",
    ])

    process_folder(["modules/skeleton_2d",
        "editor",
        "nodes",
        "resources",
    ])
    process_folder(["modules/skeleton_3d",
        "editor",
        "nodes",
        "resources",
    ])

    process_folder(["modules/steering_ai",
        "agents",
        "behaviors",
        "proximities",
    ])

    process_folder(["modules/terraman",
        "areas",
        "data",
        "level_generator",
        "library",
        "meshers",
        "meshers/blocky",
        "meshers/default",
        "nodes",
        "world",
        "world/blocky",
        "world/default",
        "world/jobs",
    ])

    process_folder(["modules/terraman_2d",
        "areas",
        "data",
        "level_generator",
        "library",
        "meshers",
        "meshers/default",
        "meshers/isometric",
        "meshers/simple",
        "nodes",
        "world",
        "world/default",
        "world/isometric",
        "world/jobs",
        "world/simple",
    ])

    process_folder(["modules/texture_packer",
        "layers",
        "rectpack2D",
        "texture_resource",
    ])

    process_folder(["modules/tile_map",
        "geometry_parser",
    ])

    process_folder(["modules/ui_extensions"])

    process_folder(["modules/unit_test"])

    process_folder(["modules/users",
        "managers",
        "singleton",
        "users",
        "web/middleware",
        "web/web_nodes",
    ])

    process_folder(["modules/voxelman",
        "areas",
        "data",
        "level_generator",
        "library",
        "meshers",
        "meshers/blocky",
        "meshers/cubic",
        "meshers/default",
        "meshers/marching_cubes",
        "nodes",
        "world",
        "world/blocky",
        "world/cubic",
        "world/default",
        "world/jobs",
        "world/marching_cubes",
    ])

    process_folder(["modules/web",
        "database",
        "editor",
        "html",
        "http",
        "nodes/admin_panel",
        "nodes/folder_serve_nodes",
        "nodes/list_page",
        "nodes/message_page",
        "nodes/paged_article",
        "nodes/redirect",
        "nodes/static_pages",
    ])

    process_folder(["modules/websocket"])

    process_folder(["modules/wfc"])

    #Editor Modules
    process_folder(["editor_modules/editor_code_editor"])

    process_folder(["editor_modules/gltf"])
    process_folder(["editor_modules/gltf/structures"])
    process_folder(["editor_modules/gltf/extensions"])
    process_folder(["editor_modules/gltf/extensions/physics"])

    process_folder(["editor_modules/plugin_refresher"])

    process_folder(["editor_modules/shader_editor"])
    process_folder(["editor_modules/text_editor"])

    #process_folder(["modules/fbx"])
    #process_folder(["modules/fbx/tools"])
    #process_folder(["modules/fbx/data"])
    #process_folder(["modules/fbx/fbx_parser"])

    process_folder(["scene"])
    process_folder(["scene/audio"])
    process_folder(["scene/debugger"])
    process_folder(["scene/2d"])
    process_folder(["scene/3d"])
    process_folder(["scene/animation"])
    process_folder(["scene/gui"])
    process_folder(["scene/main"])
    process_folder(["scene/resources"])

    process_folder(["servers"])
    process_folder(["servers/rendering"])
    process_folder(["servers/rendering/portals"])
    process_folder(["servers/physics_2d"])
    process_folder(["servers/physics"])
    process_folder(["servers/physics/joints"])
    process_folder(["servers/audio"])
    process_folder(["servers/audio/effects"])

    # Finally change back the path to the calling folder
    os.chdir(curr_folder)

    return _scu_folders