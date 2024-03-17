

def can_build(env, platform):
  return True


def configure(env):
	pass


def get_doc_classes():
    return [
        "PropDataEntry",
        "PropDataLight",
        "PropDataProp",
        "PropDataScene",
        "PropDataPortal",
        "PropDataTiledWall",
        "PropData",
        "PropDataCollisionObject",
        "PropDataStaticBody",
        "PropDataMergeGroup",

        "TiledWall",
        "TiledWallData",

        "PropDataProcessor",

        "GroundClutterFoliage",
        "GroundClutter",

        "PropESSEntity",
        "PropInstance",
        "PropMeshUtils",

        "PropSceneInstance",
        "PropMergeGroup",

        "PropUtils",

        "PropInstanceMerger",
        "PropCache",
        "PropInstanceJob",
        "PropInstanceMerger",
        "PropInstancePropJob",
        "PropMaterialCache",
        "PropMaterialCachePCM",
        "PropMesher",
        "PropMesherJobStep",
        "PropTextureJob",
        "PropLight",
    ]

def get_doc_path():
    return "doc_classes"

