import os
import platform
import sys


def can_build(env, platform):
    env.module_add_dependencies("database_sqlite", ["database"], False)

    return True

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

    return True


def configure(env):
    pass

def _configure(env):
    #env.ParseConfig("pkg-config sqlite3 --cflags --libs")

    #env.Append(CPPDEFINES=[("SQLITE_THREADSAFE", 1)])

    #env.Append(LINKFLAGS=["-ldl"])
    pass

        
def get_doc_classes():
    return [
        "SQLite3Database",
    ]

def get_doc_path():
    return "doc_classes"
