

def can_build(env, platform):
    env.module_add_dependencies("http_server_simple", ["web"], False)

    return True


def configure(env):
    pass


def get_doc_classes():
    return [
        "WebServerSimple",
    ]


def get_doc_path():
    return "doc_classes"
