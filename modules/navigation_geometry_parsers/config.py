
def can_build(env, platform):
    env.module_add_dependencies("navigation_geometry_parsers", ["navigation_mesh_generator"], False)

    return True


def configure(env):
    pass
