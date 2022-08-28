

def can_build(env, platform):
  return True


def configure(env):
	pass


def get_doc_classes():
    return [
        "Skeleton",
        "Skin",
        "SkinReference",

        "BoneAttachment",
        
        "PhysicalBone",

        "SkeletonIK",

        "SkeletonModificationStack3D",
        
        "SkeletonModification3D",
        "SkeletonModification3DCCDIK",
        "SkeletonModification3DFABRIK",
        "SkeletonModification3DJiggle",
        "SkeletonModification3DLookAt",
        "SkeletonModification3DStackHolder",
        "SkeletonModification3DTwoBoneIK",
    ]

def get_doc_path():
    return "doc_classes"
