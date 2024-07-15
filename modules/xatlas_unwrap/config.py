
def can_build(env, platform):
    return env["tools"] and platform not in ["android", "ios"]


# Todo this thould be moved here
#opts.Add(BoolVariable("builtin_xatlas", "Use the built-in xatlas library", True))

def configure(env):
    pass

def get_license_file():                                                                                                                                                                       
    return "COPYRIGHT.txt"

