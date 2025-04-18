

def can_build(env, platform):
  return True


def configure(env):
	pass


def get_doc_classes():
    return [
        "CharacterAtlasEntry",
        "CharacterAtlas",

        "ESS",

        "AuraGroup",
        "CraftRecipeHelper",
        "CraftRecipe",
        "EquipmentData",
        "ItemInstance",
        "ItemTemplate",
        "ModelVisualEntry",
        "ModelVisual",
        "SpellCooldownManipulationData",
        "SpellEffectVisualSimple",
        "SpellEffectVisual",
        "Spell",

        "ESDragAndDrop",

        "EntityAI",
        "AuraData",
        "CharacterSpec",
        "EntityClassData",
        "EntityDataContainer",
        "EntityData",
        "ItemContainerDataEntry",
        "ItemContainerData",
        "VendorItemDataEntry",
        "VendorItemData",
        "EntityResourceCostData",
        "EntityResourceCostDataHealth",
        "EntityResourceCostDataResource",
        "EntityResource",
        "EntityResourceHealth",
        "EntityResourceSpeed",
        "EntitySkillData",
        "EntitySkill",
        "ComplexLevelStatData",
        "LevelStatData",
        "SimpleLevelStatData",
        "StatData",
        "StatModifier",
        "Entity",

        "AIFormation",

        "AuraApplyInfo",
        "SpellCastInfo",

        "Bag",

        "LootDataBase",

        "SpellDamageInfo",
        "SpellHealInfo",

        "ActionBarButtonEntry",
        "ActionBarEntry",
        "ActionBarProfile",
        "InputProfileModifierEntry",
        "InputProfileModifier",
        "InputProfile",
        "ClassProfile",
        "PlayerProfile",
        "ProfileManager",

        "CharacterBones",
        "CharacterSkeleton3D",
        "CharacterSkeleton2D",
        "CharacterSkeleton",
        "EntitySpeciesData",
        "SpeciesInstance",
        "SkeletonModelEntry",
        "SpeciesModelData",

        "EntityCreateInfo",

        "SpellFollowProjectile3D",

        "ESSResourceDB",
        "ESSResourceDBFolders",
        "ESSResourceDBStatic",
        "ESSResourceDBMap",

        "EntityEnums",
        "ItemEnums",
        "SpellEnums",

        "ESSEntitySpawner",

        "PropDataEntity",
        "PropDataESSEntityWorldSpawner3D",
        "PropDataESSEntityWorldSpawner3DSingle",
        "PropDataESSEntityWorldSpawner3DArea",

        "ESSMaterialCache",
        "ESSMaterialCachePCM",

        "ESSEntityWorldSpawner3D",
        "ESSEntityWorldSpawner3DSingle",
        "ESSEntityWorldSpawner3DArea",
        "ESSEntityWorldSpawner2D",
    ]

def get_doc_path():
    return "doc_classes"

