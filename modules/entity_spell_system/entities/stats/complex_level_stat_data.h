#ifndef COMPLEX_LEVEL_STAT_DATA_H
#define COMPLEX_LEVEL_STAT_DATA_H


#include "core/object/resource.h"

#include "../../entity_enums.h"
#include "level_stat_data.h"

class ComplexLevelStatData : public LevelStatData {
	GDCLASS(ComplexLevelStatData, LevelStatData);

public:
	int get_stat_for_level(int main_stat, int level);
	void set_stat_for_level(int main_stat, int level, int value);

	int _get_stat_diff(int stat, int old_level, int new_level);

	ComplexLevelStatData();
	~ComplexLevelStatData();

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	static void _bind_methods();

private:
	Vector<Vector<int>> _stat_per_level;
};

#endif
