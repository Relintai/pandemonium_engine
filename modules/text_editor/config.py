

def can_build(env, platform):
  env.module_add_dependencies("text_editor", ["freetype"], True)

  return True


def configure(env):
	pass


def get_doc_classes():
  return []

def get_doc_path():
  return "doc_classes"
