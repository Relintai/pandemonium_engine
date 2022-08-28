

def can_build(env, platform):
  return True


def configure(env):
	pass


def get_doc_classes():
    return [
        "Skeleton2D",
        "PhysicalBone2D",

        "SkeletonModificationStack2D",
        
        "SkeletonModification2D",
        "SkeletonModification2DCCDIK",
        "SkeletonModification2DFABRIK",
        "SkeletonModification2DJiggle",
        "SkeletonModification2DLookAt",
        "SkeletonModification2DPhysicalBones",
        "SkeletonModification2DStackHolder",
        "SkeletonModification2DTwoBoneIK",
    ]

def get_doc_path():
    return "doc_classes"
