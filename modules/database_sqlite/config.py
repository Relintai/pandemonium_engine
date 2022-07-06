import os
import platform
import sys


def can_build(env, platform):
    return False

def _can_build():

#    if os.name == "posix" or sys.platform == "darwin":
#        x11_error = os.system("pkg-config --version > /dev/null")
#        if x11_error:
#            return False

#        sqlite_error = os.system("pkg-config sqlite3 --modversion --silence-errors > /dev/null ")

#        if sqlite_error:
#            print("sqlite3 not found!")
#            return False

#        print("sqlite3 found!")

#        return True

#    #todo
#    return False

    #print("sqlite3 built in!")

    return True


def configure(env):
    pass

def _configure(env):
    #env.ParseConfig("pkg-config sqlite3 --cflags --libs")

    env.Append(CPPDEFINES=["SQLITE_PRESENT"])

    # Link those statically for portability
    #if env["use_static_cpp"]:
        #env.Append(LINKFLAGS=["-static-libgcc", "-static-libstdc++"])

    env.Append(LINKFLAGS=["-ldl"])

        
