
def can_build(env, platform):
    env.module_add_dependencies("fbx", ["skeleton_3d"], False)

    return env["tools"]

def configure(env):
    pass

def get_doc_classes():
    return [
        "EditorSceneImporterFBX",
    ]

def get_doc_path():
    return "doc_classes"

def get_license_file():
  return "COPYRIGHT.txt"

