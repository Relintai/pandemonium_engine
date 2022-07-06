import os
import platform
import sys


def can_build(env, platform):
    return False

def _can_build():
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

def configure(env):
    pass

def _configure(env):
    env.ParseConfig("pkg-config libpq --cflags --libs")
    
    env.Append(CPPDEFINES=["PGSQL_PRESENT"])

    # Link those statically for portability
    #if env["use_static_cpp"]:
        #env.Append(LINKFLAGS=["-static-libgcc", "-static-libstdc++"])
