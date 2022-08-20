

def can_build(env, platform):
  return True


def configure(env):
	pass


def get_doc_classes():
    return [
      "BrightenAction",
      "BrushAction",
      "BucketAction",
      "CutAction",
      "DarkenAction",
      "LineAction",
      "MultiLineAction",
      "PaintAction",
      "PasteCutAction",
      "PencilAction",
      "RainbowAction",
      "RectAction",

      "PaintWindow",
    ]

def get_doc_path():
    return "doc_classes"
