

def can_build(env, platform):
  return True


def configure(env):
	pass


def get_doc_classes():
    return [
        "MMAlgos",
        "MMNode",
        "CurveBase ",
        "GradientBase",
        "PolygonBase ",
        "MMNodeUniversalProperty",
        "MMMaterial",
        "MMGraphNode",
        "MatMakerGDEditor",

        "MMUniform",
        "MMGreyscaleUniform",
    ]

def get_doc_path():
    return "doc_classes"

