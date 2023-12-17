#ifndef PROP_DATA_LIGHT_H
#define PROP_DATA_LIGHT_H


#include "core/math/color.h"

#include "prop_data_entry.h"

class PropDataLight : public PropDataEntry {
	GDCLASS(PropDataLight, PropDataEntry);

public:
	Color get_light_color() const;
	void set_light_color(const Color value);

	int get_light_size() const;
	void set_light_size(const int value);

	bool _processor_handles(Node *node);
	void _processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform);
	Node *_processor_get_node_for(const Transform &transform);

	PropDataLight();
	~PropDataLight();

protected:
	static void _bind_methods();

private:
	Color _light_color;
	int _light_size;
};

#endif
