

def can_build(env, platform):
    env.module_add_dependencies("opus", ["ogg"])
    return True


def configure(env):
    pass
