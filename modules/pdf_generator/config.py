

def can_build(env, platform):
    return True
    #return False

def configure(env):
    pass

def get_doc_classes():
    return [
        "PDFDocument",
        "PDFPage",
        "PDFFont",
        "PDFImage",
        "PDFOutline",
        "PDFDestination",
        "PDF3DMeasure",
        "PDFEmbeddedFile",
        "PDFExData",
        "PDFExtGState",
        "PDFShading",
        "PDFXObject",
        "PDFEncoder",
        "PDFDict",
        "PDFDate",
        "PDFDashMode",
        "PDFU3D",
        "PDFJavascript",
        "PDF3DView",
        "PDF3DViewNode",
        "PDFAnnotation",
        "PDFAnnotation3D",
        "PDFAnnotationText",
        "PDFAnnotationFreeText",
        "PDFAnnotationLine",
        "PDFAnnotationWidgetWhitePrint",
        "PDFAnnotationWidget",
        "PDFAnnotationLink",
        "PDFAnnotationURILink",
        "PDFAnnotationTextMarkup",
        "PDFAnnotationHighlight",
        "PDFAnnotationUnderline",
        "PDFAnnotationSquiggly",
        "PDFAnnotationStrikeOut",
        "PDFAnnotationPopup",
        "PDFAnnotationStamp",
        "PDFAnnotationProjection",
        "PDFAnnotationSquare",
        "PDFAnnotationCircle",
    ]

def get_doc_path():
    return "doc_classes"

def get_license_file():
  return "COPYRIGHT.txt"

