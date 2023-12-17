#ifndef PROP_2D_UTILS_H
#define PROP_2D_UTILS_H


#include "core/object/object.h"
#include "core/object/reference.h"
#include "core/containers/vector.h"
#include "scene/main/spatial.h"

#include "scene/main/node.h"

class Prop2DData;
class Prop2DDataEntry;

class Room;

class Prop2DUtils : public Object {
	GDCLASS(Prop2DUtils, Object);

public:
	static Prop2DUtils *get_singleton();

	Ref<Prop2DData> convert_tree(Node *root);
	void _convert_tree(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform);

	static int add_processor(const Ref<Prop2DDataEntry> &processor);
	static Ref<Prop2DDataEntry> get_processor(const int index);
	static void swap_processors(const int index1, const int index2);
	static void remove_processor(const int index);
	static int get_processor_count();

	Prop2DUtils();
	~Prop2DUtils();

protected:
	static void _bind_methods();

private:
	int _add_processor_bind(const Ref<Prop2DDataEntry> &processor);
	Ref<Prop2DDataEntry> _get_processor_bind(const int index);
	void _swap_processors_bind(const int index1, const int index2);
	void _remove_processor_bind(const int index);
	int _get_processor_count_bind();

	static Vector<Ref<Prop2DDataEntry>> _processors;
	static Prop2DUtils *_instance;
};

#endif
