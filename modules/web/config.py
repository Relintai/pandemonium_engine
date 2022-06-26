

def can_build(env, platform):
    return True
    #return False


def configure(env):
    pass


def get_doc_classes():
    return [
        #"WorldArea",

        "HTTPServerEnums",
        "WebServerCookie",
        "HTMLBuilder",
        "HTMLTag",
    ]


def get_doc_path():
    return "doc_classes"
