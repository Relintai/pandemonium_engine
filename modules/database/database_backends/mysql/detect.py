import os
import platform
import sys


def is_active():
    return True


def get_name():
    return "mysql"


def can_build():

    if os.name == "posix" or sys.platform == "darwin":
        x11_error = os.system("pkg-config --version > /dev/null")
        if x11_error:
            return False

        mariadb_error = os.system("pkg-config mariadb --modversion --silence-errors > /dev/null ")
        mysql_error = os.system("pkg-config mysql --modversion --silence-errors > /dev/null ")

        if mariadb_error and mysql_error:
            print("mysql and mariadb not found..")
            return False

        if not mariadb_error:
            print("mariadb found!")

            return True
            
        if not mysql_error:
            print("mysql found!")

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
    mariadb_error = os.system("pkg-config mariadb --modversion --silence-errors > /dev/null ")
    mysql_error = os.system("pkg-config mysql --modversion --silence-errors > /dev/null ")

    if not mariadb_error:
        env.ParseConfig("pkg-config mariadb --cflags --libs")
        env.Append(CPPDEFINES=["MYSQL_PRESENT"])
        return

    if not mysql_error:
        env.ParseConfig("pkg-config mysql --cflags --libs")

    env.Append(CPPDEFINES=["MYSQL_PRESENT"])

    # Link those statically for portability
    #if env["use_static_cpp"]:
        #env.Append(LINKFLAGS=["-static-libgcc", "-static-libstdc++"])
