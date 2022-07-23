

def can_build(env, platform):
    return True


def configure(env):
    pass


def get_doc_classes():
    return [
        "User",
        "UserModule",

        "UserManager",
        "UserManagerStatic",
        "UserManagerFile",
        "UserDB",

        "UserSessionSetupWebServerMiddleware",

    	"UserWebPage",
	    "UserDeleteWebPage",
	    "UserLoginWebPage",
	    "UserLogoutWebPage",
	    "UserPasswordResetWebPage",
	    "UserRegisterWebPage",
	    "UserSettingsWebPage",
	    "UserStatsWebPage",
    ]


def get_doc_path():
    return "doc_classes"

# This is (or should be if not) an optional dependency, it's needed so the web module is built first, as some classes need to inherit from WebNode (only when web module is enabled).
def get_module_dependencies():
    return [
        "web",
    ]

