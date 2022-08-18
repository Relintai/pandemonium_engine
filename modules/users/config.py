

def can_build(env, platform):
    env.module_add_dependencies("users", ["web"], True)

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
        "UserManagerDB",
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

