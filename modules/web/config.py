

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
		"WebServerRequestScriptable",

        "CSRFTokenWebServerMiddleware",
        "LocaleSetupWebServerMiddleware",

        #"HTTPParser",
        #"HTTPWriter",

        "HTMLBuilder",
        "HTMLTag",
        "HTMLPaginator",

        "FormFieldEntry",
    	"FormExistsFieldEntry",
	    "FormIntFieldEntry",
    	"FormFloatFieldEntry",
	    "FormAlphaFieldEntry",
    	"FormAlphaNumericFieldEntry",
	    "FormNeedsLowercaseCharacterFieldEntry",
    	"FormNeedsUppercaseCharacterFieldEntry",
	    "FormNeedsOtherCharacterFieldEntry",
    	"FormMinimumLengthFieldEntry",
	    "FormMaximumLengthFieldEntry",
    	"FormEmailFieldEntry",
	    "FormNeedToMatchOtherFieldEntry",
    	"FormField",
	    "FormValidator",

	    "HTMLParserAttribute",
	    "HTMLParserTag",
	    "HTMLParser",
	    
	    "HTMLTemplate",
	    "HTMLTemplateData",
        "HTMLTemplateMultilang",

    	"BBCodeParserAttribute",
	    "BBCodeParserTag",
	    "BBCodeParser",

	    "MarkdownRenderer",
		"MarkdownRendererCustomRendererCallback",

        "StaticWebPage",
        "StaticWebPageFile",
        "StaticWebPageFolderFiles",

        "FolderServeWebPage",
        "BrowsableFolderServeWebPage",

        "ListWebPage",

        "PagedArticleWebPage",
        "PagedArticlesWebPage",
        "PagedArticlesWebPageMDIndex",

	    "RedirectWebPage",
	    "AliasWebPage",

		"HTTPSessionManagerDB",
    ]


def get_doc_path():
    return "doc_classes"

def get_license_file():
  return "COPYRIGHT.txt"

