

def can_build(env, platform):
    return True
    #return False


def configure(env):
    pass


def get_doc_classes():
    return [
        "FileCache",

        "HTTPServerEnums",
        "WebServerCookie",
	    "CSRFTokenWebServerMiddleware",
	    "HTTPSession",
	    "HTTPSessionManager",
	    "SessionSetupWebServerMiddleware",
	    "WebNode",
	    "WebPermission",
    	"WebRoot",
	    "WebServer",
	    "WebServerCookie",
	    "WebServerMiddleware",
	    "WebServerRequest",

        "HTMLBuilder",
        "HTMLTag",

        "WebServerSimple",
    ]


def get_doc_path():
    return "doc_classes"
