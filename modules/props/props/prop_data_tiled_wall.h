#ifndef PROP_DATA_TILED_WALL_H
#define PROP_DATA_TILED_WALL_H


#include "core/math/vector3.h"
#include "prop_data_entry.h"

class TiledWallData;

class PropDataTiledWall : public PropDataEntry {
	GDCLASS(PropDataTiledWall, PropDataEntry);

public:
	int get_width() const;
	void set_width(const int value);

	int get_heigth() const;
	void set_heigth(const int value);

	Ref<TiledWallData> get_data();
	void set_data(const Ref<TiledWallData> &data);

	bool get_collision() const;
	void set_collision(const bool value);

	bool _processor_handles(Node *node);
	void _processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform);
	Node *_processor_get_node_for(const Transform &transform);

	PropDataTiledWall();
	~PropDataTiledWall();

protected:
	static void _bind_methods();

private:
	int _width;
	int _height;
	bool _collision;

	Ref<TiledWallData> _data;
};

#endif
