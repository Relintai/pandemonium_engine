
def can_build(env, platform):
    return True

def configure(env):
    pass


def get_doc_classes():
    return [
        "@CScript",
        "CScript",
    ]


def get_doc_path():
    return "doc_classes"


def is_enabled():
    # The module is disabled by default.
    return False