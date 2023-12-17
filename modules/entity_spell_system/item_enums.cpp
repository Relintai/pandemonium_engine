

#include "item_enums.h"

const String ItemEnums::BINDING_STRING_RARITY = "None,Common,Uncommon,Superior,Heroic,Mythic,Artifact";
const String ItemEnums::BINDING_STRING_RARITY_FLAG = "Common,Uncommon,Superior,Heroic,Mythic,Artifact";
const String ItemEnums::BINDING_STRING_ITEM_TYPE = "None,Equipment,Potion,Herb,Or,Gemstone,Food,Alchemy,Engineering,Enchanting,Tailoring,Recipe,Currency,Bag";
const String ItemEnums::BINDING_STRING_ITEM_TYPE_FLAGS = "Equipment,Potion,Herb,Or,Gemstone,Food,Alchemy,Engineering,Enchanting,Tailoring,Recipe,Currency,Bag";
const String ItemEnums::BINDING_STRING_ITEM_SUB_TYPE = "None,Sword,Axe,Mace,Dagger,Bow,Crossbow,Gun,Wand,Quest Item";
const String ItemEnums::BINDING_STRING_ITEM_SUB_SUB_TYPE = "None,Two Hand,One Hand,Left Hand,Right Hand";
const String ItemEnums::BINDING_STRING_ARMOR_TYPE = "None,Cloth,Leather,Mail,Plate";

String ItemEnums::get_armor_type_string(int type) {
	switch (type) {
		case 0:
			return "none";
		case 1:
			return "cloth";
		case 2:
			return "leather";
		case 3:
			return "mail";
		case 4:
			return "plate";
	}

	return "";
}
