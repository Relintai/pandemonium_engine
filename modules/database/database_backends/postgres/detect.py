import os
import platform
import sys


def is_active():
    return True


def get_name():
    return "pgsql"


def can_build():

    if os.name == "posix" or sys.platform == "darwin":
        x11_error = os.system("pkg-config --version > /dev/null")
        if x11_error:
            return False

        libpg_error = os.system("pkg-config libpq --modversion --silence-errors > /dev/null ")

        if libpg_error:
            print("postgres not found!")
            return False

        print("postgres found!")

        return True

    #todo
    return False


def get_opts():
    from SCons.Variables import BoolVariable, EnumVariable

    return [
        EnumVariable("debug_symbols", "Add debugging symbols to release/release_debug builds", "yes", ("yes", "no")),
    ]


def get_flags():

    return []


def configure(env):
    env.ParseConfig("pkg-config libpq --cflags --libs")
    
    env.Append(CPPDEFINES=["PGSQL_PRESENT"])

    # Link those statically for portability
    #if env["use_static_cpp"]:
        #env.Append(LINKFLAGS=["-static-libgcc", "-static-libstdc++"])
