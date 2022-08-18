
def can_build(env, platform):
    env.module_add_dependencies("vorbis", ["ogg"])
    return True


def configure(env):
    pass
