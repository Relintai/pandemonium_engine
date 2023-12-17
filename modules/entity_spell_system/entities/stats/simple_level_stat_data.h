#ifndef SIMPLE_LEVEL_STAT_DATA_H
#define SIMPLE_LEVEL_STAT_DATA_H


#include "level_stat_data.h"

class SimpleLevelStatData : public LevelStatData {
	GDCLASS(SimpleLevelStatData, LevelStatData);

public:
	int get_stat_per_level(int main_stat);
	void set_stat_per_level(int main_stat, int value);

	int _get_stat_diff(int stat, int old_level, int new_level);

	SimpleLevelStatData();
	~SimpleLevelStatData();

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	static void _bind_methods();

private:
	Vector<int> _stat_per_level;
};

#endif
