def can_build(env, platform):
    return True

def configure(env):
    pass

def get_doc_classes():
    return [
        "GSAIUtils",

        "GSAITargetAcceleration",
        "GSAISteeringBehavior",
        "GSAISteeringAgent",
        "GSAIPath",
        "GSAIGroupBehavior",
        "GSAIAgentLocation",
        "GSAIProximity",

        "GSAIRadiusProximity",
        "GSAIInfiniteProximity",

        "GSAIArrive",
        "GSAIAvoidCollisions",
        "GSAIBlend",
        "GSAICohesion",
        "GSAIEvade",
        "GSAIFace",
        "GSAIFlee",
        "GSAIFollowPath",
        "GSAILookWhereYouGo",
        "GSAIMatchOrientation",
        "GSAIPriority",
        "GSAIPursue",
        "GSAISeek",
        "GSAISeparation",
    ]

def get_doc_path():
    return "doc_classes"

def get_license_file():
  return "COPYRIGHT.txt"
