"""Functions used to generate source files during build time

All such functions are invoked in a subprocess on Windows to prevent build flakiness.

"""
from platform_methods import subprocess_main
from collections import OrderedDict

def make_default_controller_mappings(target, source, env):
    dst = target[0]
    g = open(dst, "w")

    g.write("/* THIS FILE IS GENERATED DO NOT EDIT */\n")
    g.write('#include "core/typedefs.h"\n')
    g.write('#include "core/input/default_controller_mappings.h"\n')

    # ensure mappings have a consistent order
    platform_mappings = OrderedDict()
    for src_path in source:
        with open(src_path, "r") as f:
            # read mapping file and skip header
            mapping_file_lines = f.readlines()[2:]

        current_platform = None
        for line in mapping_file_lines:
            if not line:
                continue
            line = line.strip()
            if len(line) == 0:
                continue
            if line[0] == "#":
                current_platform = line[1:].strip()
                if current_platform not in platform_mappings:
                    platform_mappings[current_platform] = {}
            elif current_platform:
                line_parts = line.split(",")
                guid = line_parts[0]
                if guid in platform_mappings[current_platform]:
                    g.write(
                        "// WARNING - DATABASE {} OVERWROTE PRIOR MAPPING: {} {}\n".format(
                            src_path, current_platform, platform_mappings[current_platform][guid]
                        )
                    )
                platform_mappings[current_platform][guid] = line

    platform_variables = {
        "Linux": "#ifdef X11_ENABLED",
        "Windows": "#ifdef WINDOWS_ENABLED",
        "Mac OS X": "#ifdef OSX_ENABLED",
        "Android": "#ifdef ANDROID_ENABLED",
        "iOS": "#ifdef IPHONE_ENABLED",
        "Javascript": "#ifdef JAVASCRIPT_ENABLED",
        "UWP": "#ifdef UWP_ENABLED",
    }

    g.write("const char* DefaultControllerMappings::mappings[] = {\n")
    for platform, mappings in platform_mappings.items():
        variable = platform_variables[platform]
        g.write("{}\n".format(variable))
        for mapping in mappings.values():
            g.write('\t"{}",\n'.format(mapping))
        g.write("#endif\n")

    g.write("\tNULL\n};\n")
    g.close()


if __name__ == "__main__":
    subprocess_main(globals())
