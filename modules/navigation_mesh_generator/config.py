

def can_build(env, platform):
    env.module_add_dependencies("navigation_mesh_generator", ["navigation"], False)
    
    return True


def configure(env):
    pass
