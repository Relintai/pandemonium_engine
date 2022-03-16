

def can_build(env, platform):
    return True


def configure(env):
    pass


def get_doc_classes():
    return [
        "Terrain2DWorldArea",

        "Terrain2DLight",
        "Terrain2DLightNode",

        "Terrain2DLevelGenerator",
        "Terrain2DLevelGeneratorFlat",

        "Terrain2DSurfaceMerger",
        "Terrain2DSurfaceSimple",
        "Terrain2DSurface",
        "Terrain2DLibraryMerger",
        "Terrain2DLibrarySimple",
        "Terrain2DLibrary",
        "Terrain2DLibraryMergerPCM",

        "Terrain2DMaterialCache",
        "Terrain2DMaterialCachePCM",

        "TerraCubePoints",
        "Terrain2DMesherCubic",
        "Terrain2DMeshData",

        "MarchingCubesCellData",
        "Terrain2DMesherMarchingCubes",

        "Terrain2DMesher",

        "EnvironmentData",
        "Terrain2DChunk",
        "Terrain2DChunkDefault",
        "Terrain2DStructure",
        "BlockTerrain2DStructure",
        "Terrain2DWorld",

        "Terrain2DMesherSimple",
        "Terrain2DWorldSimple",
        "Terrain2DChunkSimple",

        "Terrain2DMesherIsometric",
        "Terrain2DWorldIsometric",
        "Terrain2DChunkIsometric",

        "Terrain2DWorldMarchingCubes",
        "Terrain2DChunkMarchingCubes",

        "Terrain2DMesherCubic",
        "Terrain2DWorldCubic",
        "Terrain2DChunkCubic",

        "Terrain2DMesherDefault",
        "Terrain2DWorldDefault",

        "Terrain2DJob",
        "Terrain2DTerrain2DJob",
        "Terrain2DLightJob",
        "Terrain2DProp2DJob",

        "Terrain2DEnvironmentData",
        "Terrain2DMesherJobStep",

    ]


def get_doc_path():
    return "doc_classes"
