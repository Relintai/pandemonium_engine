

def can_build(env, platform):
  if not env["tools"]:
    return False

  env.module_add_dependencies("text_editor", ["freetype"], True)
  env.module_add_dependencies("text_editor", ["code_editor"], False)

  return True


def configure(env):
	pass


def get_doc_classes():
  return [
    "TextEditorFile",
  ]

def get_doc_path():
  return "doc_classes"
