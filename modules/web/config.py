

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

        #"HTTPParser",
        #"HTTPWriter",

        "HTMLBuilder",
        "HTMLTag",
        "HTMLPaginator",

        "WebServerSimple",

        "StaticPage",
        "StaticPageFile",
        "StaticPageFolderFiles",

        "FolderServeNode",
        "BrowsableFolderServeNode",

        "ListPage",
    ]


def get_doc_path():
    return "doc_classes"
