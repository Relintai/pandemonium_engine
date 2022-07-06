import os

def can_build(env, platform):
    return False

def _can_build():
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


def configure(env):
    pass

def _configure(env):
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


def get_doc_classes():
    return [
    ]

def get_doc_path():
    return "doc_classes"

