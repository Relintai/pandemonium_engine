def can_build(env, platform):
  return True
  
  
def configure(env):
  pass
 

def get_doc_classes():
    return [
        "GDDragonBones",
        "GDArmatureDisplay",
        "GDDisplay",
        "GDBone2D",
        "GDDragonBonesResource",
        "GDSlot",
        "GDOwnerNode",
        "GDMesh"
    ]


def get_doc_path():
    return "doc_classes"