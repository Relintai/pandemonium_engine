def can_build(env, platform):
    return True


def configure(env):
    pass


def get_doc_classes():
    return [
        "WaveFormCollapse",
        "TilingWaveFormCollapse",
        "OverlappingWaveFormCollapse",
    ]


def get_doc_path():
    return "doc_classes"


def is_enabled():
    return True
