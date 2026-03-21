

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
        "PDFAnnotation",
        "PDF3DMeasure",
        "PDFEmbeddedFile",
        "PDFExData",
        "PDFExtGState",
        "PDFShading",
        "PDFXObject",
        "PDFEncoder",
    ]

def get_doc_path():
    return "doc_classes"

def get_license_file():
  return "COPYRIGHT.txt"

