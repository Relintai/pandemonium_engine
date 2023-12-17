#ifndef ESS_RESOURCE_DB_FOLDERS_H
#define ESS_RESOURCE_DB_FOLDERS_H


#include "core/variant/array.h"
#include "core/bind/core_bind.h"
#include "core/config/engine.h"
#include "core/containers/hash_map.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"
#include "core/containers/vector.h"

#include "ess_resource_db_map.h"

#include "scene/main/node.h"

#include "../item_enums.h"

#include "../defines.h"

class Spell;
class EntityData;
class CraftRecipe;
class ItemTemplate;
class EntityResource;
class EntitySkillData;
class EntityCreateInfo;
class SpellCastInfo;
class EntitySpeciesData;

class ESSResourceDBFolders : public ESSResourceDBMap {
	GDCLASS(ESSResourceDBFolders, ESSResourceDBMap);

public:
	bool get_automatic_load() const;
	void set_automatic_load(const bool load);

	bool get_load_folders() const;
	void set_load_folders(const bool load);

	PoolStringArray get_folders() const;
	void set_folders(const PoolStringArray &folders);

	virtual void _initialize();
	void load_all();
	void load_folders();
	void load_folder(const String &folder);
	void add_resource(const Ref<Resource> &resource);

	Ref<Resource> load_resource(const String &path, const String &type_hint = "");

	ESSResourceDBFolders();
	~ESSResourceDBFolders();

protected:
	static void _bind_methods();

private:
	String _xp_data_path;

	PoolStringArray _folders;

	bool _automatic_load;
	bool _load_folders;
};

#endif
