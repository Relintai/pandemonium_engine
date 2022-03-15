

def can_build(env, platform):
  return True


def configure(env):
	pass


def get_doc_classes():
    return [
        "Prop2DDataEntry",
        "Prop2DDataLight",
        "Prop2DDataProp2D",
        "Prop2DDataScene",
        "Prop2DDataPortal",
        "Prop2DDataTiledWall2D",
        "Prop2DData",
        
        "TiledWall2D",
        "TiledWall2DData",

        "Prop2DDataProcessor",

        "GroundClutter2DFoliage",
        "GroundClutter2D",

        "Prop2DESSEntity",
        "Prop2DInstance",
        "Prop2DMeshUtils",

        "Prop2DSceneInstance",

        "Prop2DUtils",

        "Prop2DInstanceMerger",
        "Prop2DCache",
        "Prop2DInstanceJob",
        "Prop2DInstanceMerger",
        "Prop2DInstanceProp2DJob",
        "Prop2DMaterialCache",
        "Prop2DMaterialCachePCM",
        "Prop2DMesher",
        "Prop2DMesherJobStep",
        "Prop2DTextureJob",
        "Prop2DLight",
    ]

def get_doc_path():
    return "doc_classes"

