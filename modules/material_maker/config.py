

def can_build(env, platform):
  return True


def configure(env):
	pass


def get_doc_classes():
    return [
        "MMAlgos",
        "MMNode",
        "CurveBase",
        "GradientBase",
        "PolygonBase",
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
        "MMWarp",

        "MMShape",
        "MMImage",
        "MMCurve",

        "MMSdf3dRender",
        "MMSdf3dColor",

        "MMSdf3dTfTranslate",
        "MMSdf3dTfScale",
        "MMSdf3dTfRotate",

        "MMSdf3dShapeTorus",
        "MMSdf3dShapeSphere",
        "MMSdf3dShapeCylinder",
        "MMSdf3dShapeCone",
        "MMSdf3dShapeCapsule",
        "MMSdf3dShapeBox",

        "MMSdf3dOpSmoothBool",
        "MMSdf3dOpRounded",
        "MMSdf3dOpRevolution",
        "MMSdf3dOpRepeat",
        "MMSdf3dOpMorph",
        "MMSdf3dOpElongation",
        "MMSdf3dOpCircleRepeat",
        "MMSdf3dOpBool",

        "SSSdShow",

        "MMSdTfTranslate",
        "MMSdTfScale",
        "MMSdTfRotate",

        "MMSdShapeRhombus",
        "MMSdShapePolygon",
        "MMSdShapeLine",
        "MMSdShapeCircle",
        "MMSdShapeBox",
        "MMSdShapeArc",

        "MMSdOpSmoothBool",
        "MMSdOpRoundedShape",
        "MMSdOpRepeat",
        "MMSdOpMorph",
        "MMSdOpCircleRepeat",
        "MMSdOpBool",
        "MMSdOpAnnularShape",

        "MMWeave",
        "MMTruchet",
        "MMSineWave",
        "MMScratches",
        "MMRunes",
        "MMPattern",
        "MMIching",
        "MMBricks",
        "MMBeehive",

        "MMOutputImage",

        "MMVoronoi",
        "MMNoise",
        "MMFbmNoise",
        "MMColorValue",
        "MMColorNoise",
        "MMAnisotropicNoise",

        "MMRadialGradient",
        "MMGradient",
        "MMCircularGradient",

        "MMTonality",
        "MMSwapChannels",
        "MMQuantize",
        "MMMath",
        "MMMakeTileable",
        "MMInvert",
        "MMGreyscale",
        "MMFillToUv",
        "MMFillToSize",
        "MMFillToRandomGrey",

        "MMFillToRandomColor",
        "MMFillToPosition",
        "MMFillToColor",
        "MMFillChannel",
        "MMEmboss",
        "MMDecompose",
        "MMCombine",
        "MMColorize",
        "MMBrightnessContrast",
        "MMBlurGaussian",
        "MMBlurSlope",
        "MMBlend",
        "MMAdjustHsv",
        "MMTonesStep",
    ]

def get_doc_path():
    return "doc_classes"

def get_license_file():
  return "COPYRIGHT.txt"
