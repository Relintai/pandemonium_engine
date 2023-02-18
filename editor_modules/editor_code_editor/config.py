

def can_build(env, platform):
  if not env["tools"]:
    return False
      
  env.module_add_dependencies("editor_code_editor", ["freetype"], True)

  return True

def configure(env):
	pass


def get_doc_classes():
    return [
    ]

def get_doc_path():
    return "doc_classes"
