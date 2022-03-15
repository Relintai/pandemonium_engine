

def can_build(env, platform):
  return True


def configure(env):
	pass


def get_doc_classes():
    return [
        "MeshDataResource",
        "MeshDataInstance",

        "MeshDataInstanceProcessor",
        "PropDataMeshData",
        "MeshDataResourceCollection"
    ]

def get_doc_path():
    return "doc_classes"
