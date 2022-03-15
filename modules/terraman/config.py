

def can_build(env, platform):
    return True


def configure(env):
    pass


def get_doc_classes():
    return [
        "TerrainWorldArea",

        "TerrainLight",
        "TerrainLightNode",

        "TerrainLevelGenerator",
        "TerrainLevelGeneratorFlat",

        "TerrainSurfaceMerger",
        "TerrainSurfaceSimple",
        "TerrainSurface",
        "TerrainLibraryMerger",
        "TerrainLibrarySimple",
        "TerrainLibrary",
        "TerrainLibraryMergerPCM",

        "TerrainMaterialCache",
        "TerrainMaterialCachePCM",

        "TerraCubePoints",
        "TerrainMesherCubic",
        "TerrainMeshData",

        "MarchingCubesCellData",
        "TerrainMesherMarchingCubes",

        "TerrainMesher",

        "EnvironmentData",
        "TerrainChunk",
        "TerrainChunkDefault",
        "TerrainStructure",
        "BlockTerrainStructure",
        "TerrainWorld",

        "TerrainMesherBlocky",
        "TerrainWorldBlocky",
        "TerrainChunkBlocky",

        "TerrainWorldMarchingCubes",
        "TerrainChunkMarchingCubes",
    
        "TerrainMesherCubic",
        "TerrainWorldCubic",
        "TerrainChunkCubic",

        "TerrainMesherDefault",
        "TerrainWorldDefault",

        "TerrainJob",
        "TerrainTerrainJob",
        "TerrainLightJob",
        "TerrainPropJob",

        "TerrainEnvironmentData",
        "TerrainMesherJobStep",
        
    ]


def get_doc_path():
    return "doc_classes"
