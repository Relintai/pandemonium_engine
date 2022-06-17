

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

        "MMTranslate",
        "MMTransform",
        "MMTiler",
        "MMShear",
        "MMScale",
        "MMRotate",
        "MMRepeat",
        "MMMirror",
        "MMKaleidoscope",
        "MMColorTiler",
        "MMCircleMap",

        "MMShape",
        "MMImage",
        "MMCurve",

        "MMSdf3dTfTranslate",
        "MMSdf3dTfScale",
        "MMSdf3dTfRotate",

        "MMSdf3dShapeTorus",
    ]

def get_doc_path():
    return "doc_classes"

