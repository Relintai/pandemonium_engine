

def can_build(env, platform):
  if not env["tools"]:
    return False

  env.module_add_dependencies("shader_editor", ["editor_code_editor"], False)

  return True

def configure(env):
	pass


def get_doc_classes():
    return [
    ]

def get_doc_path():
    return "doc_classes"
