#ifndef STAT_DATA_H
#define STAT_DATA_H


#include "core/object/resource.h"

#include "scene/resources/curve.h"

#include "level_stat_data.h"

class StatData : public Resource {
	GDCLASS(StatData, Resource);

public:
	struct ModStat {
		int target_stat_id;
		int stat_id;
		float multiplier;
	};

	enum {
		MAX_MOD_STATS = 8,
	};

public:
	float get_base(int index);
	void set_base(int index, float entry);

	Ref<LevelStatData> get_level_stat_data();
	void set_level_stat_data(Ref<LevelStatData> value);

	bool has_mod_stats();

	int get_mod_stat_count();
	void set_mod_stat_count(int value);

	int get_target_stat_id(int index);
	void set_target_stat_id(int index, int value);

	int get_mod_stat_id(int index);
	void set_mod_stat_id(int index, int value);

	float get_mod_stat_multiplier(int index);
	void set_mod_stat_multiplier(int index, float value);

	StatData();
	~StatData();

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	void _validate_property(PropertyInfo &property) const;
	static void _bind_methods();

private:
	Vector<float> _entries;

	Ref<LevelStatData> _level_stat_data;

	int _mod_stat_count;
	ModStat _mod_stats[MAX_MOD_STATS];
};

#endif
