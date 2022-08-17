#ifndef POLYGON_BASE_H
#define POLYGON_BASE_H

#include "core/math/vector2.h"
#include "core/variant/variant.h"

#include "../mm_node.h"

class PolygonBase : public MMNode {
	GDCLASS(PolygonBase, MMNode);

public:
	void clear();

	void add_point(const float x, const float y, const bool closed = true);
	bool remove_point(const int index);

	int get_point_count();
	Vector2 get_point(const int i);

	void set_point(const int i, const Vector2 &v);

	PoolVector2Array get_points();
	void set_points(const PoolVector2Array &v);

	void polygon_changed();
	void _polygon_changed();

	String _to_string();

	PolygonBase();
	~PolygonBase();

protected:
	static void _bind_methods();

	PoolVector2Array points;
};

#endif
