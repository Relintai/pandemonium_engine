#ifndef PROP_UTILS_H
#define PROP_UTILS_H


#include "core/object/object.h"
#include "core/object/reference.h"
#include "core/containers/vector.h"
#include "scene/main/spatial.h"

#include "scene/main/node.h"

class PropData;
class PropDataEntry;

class Room;

class PropUtils : public Object {
	GDCLASS(PropUtils, Object);

public:
	static PropUtils *get_singleton();

	Ref<PropData> convert_tree(Node *root);
	void _convert_tree(Ref<PropData> prop_data, Node *node, const Transform &transform);

	bool generate_room_points_node(Node *node);
	void generate_room_points(Room *room);
	void get_mesh_arrays(Node *node, Vector<PoolVector<Vector3>> *arrs);
	bool is_plane_unique(const PoolVector<Plane> &planes, const Plane &p);

	static int add_processor(const Ref<PropDataEntry> &processor);
	static Ref<PropDataEntry> get_processor(const int index);
	static void swap_processors(const int index1, const int index2);
	static void remove_processor(const int index);
	static int get_processor_count();

	PropUtils();
	~PropUtils();

protected:
	static void _bind_methods();

private:
	int _add_processor_bind(const Ref<PropDataEntry> &processor);
	Ref<PropDataEntry> _get_processor_bind(const int index);
	void _swap_processors_bind(const int index1, const int index2);
	void _remove_processor_bind(const int index);
	int _get_processor_count_bind();

	static Vector<Ref<PropDataEntry>> _processors;
	static PropUtils *_instance;
};

#endif
