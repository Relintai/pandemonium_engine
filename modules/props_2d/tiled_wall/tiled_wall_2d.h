#ifndef TILED_WALL_2D_H
#define TILED_WALL_2D_H


#include "scene/resources/texture.h"

#include "scene/main/node_2d.h"

#include "core/math/vector3.h"

class TiledWall2DData;
class Prop2DMaterialCache;
class Prop2DMesher;

class TiledWall2D : public Node2D {
	GDCLASS(TiledWall2D, Node2D);

public:
	int get_width() const;
	void set_width(const int value);

	int get_heigth() const;
	void set_heigth(const int value);

	Ref<TiledWall2DData> get_data();
	void set_data(const Ref<TiledWall2DData> &data);

	Rect2 get_rect() const;
	PoolVector<Face3> get_faces(uint32_t p_usage_flags) const;

	Transform2D get_mesh_transform() const;
	void set_mesh_transform(const Transform2D &value);

#ifdef TOOLS_ENABLED
	virtual bool _edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const;

	virtual bool _edit_use_rect() const;
	virtual Rect2 _edit_get_rect() const;
#endif

	void refresh();
	void generate_mesh();
	void clear_mesh();
	void free_mesh();

	void draw();

	TiledWall2D();
	~TiledWall2D();

protected:
	void _notification(int p_what);
	static void _bind_methods();

private:
	int _width;
	int _height;

	Ref<TiledWall2DData> _data;
	Ref<Prop2DMaterialCache> _cache;
	Ref<Prop2DMesher> _mesher;
	AABB _aabb;

	RID _mesh_rid;
	RID _texture_rid;

	Rect2 _rect;

	Array _mesh_array;

	Transform2D _mesh_transform;

#ifdef TOOLS_ENABLED
	Vector<Point2> _editor_selection_points;
#endif
};

#endif
