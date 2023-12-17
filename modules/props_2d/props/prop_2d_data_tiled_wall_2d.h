#ifndef PROP_2D_DATA_TILED_WALL_H
#define PROP_2D_DATA_TILED_WALL_H


#include "core/math/vector3.h"
#include "prop_2d_data_entry.h"

class TiledWall2DData;

class Prop2DDataTiledWall2D : public Prop2DDataEntry {
	GDCLASS(Prop2DDataTiledWall2D, Prop2DDataEntry);

public:
	int get_width() const;
	void set_width(const int value);

	int get_heigth() const;
	void set_heigth(const int value);

	Ref<TiledWall2DData> get_data();
	void set_data(const Ref<TiledWall2DData> &data);

	bool get_collision() const;
	void set_collision(const bool value);

	bool _processor_handles(Node *node);
	void _processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform, Ref<Prop2DDataEntry> entry = Ref<Prop2DDataEntry>());
	Node *_processor_get_node_for(const Transform2D &transform, Node *node = nullptr);

	Prop2DDataTiledWall2D();
	~Prop2DDataTiledWall2D();

protected:
	static void _bind_methods();

private:
	int _width;
	int _height;
	bool _collision;

	Ref<TiledWall2DData> _data;
};

#endif
