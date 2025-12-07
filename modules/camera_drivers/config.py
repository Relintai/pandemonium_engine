
def can_build(env, platform):
    env.module_add_dependencies("camera_drivers", ["camera_server"], False)

    import sys

    if sys.platform.startswith("freebsd") or sys.platform.startswith("openbsd"):
        return False

    return platform == "osx" or platform == "windows" or platform == "x11" or platform == "android"


def configure(env):
    pass

