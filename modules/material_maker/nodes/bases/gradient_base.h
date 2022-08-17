#ifndef GRADIENT_BASE_H
#define GRADIENT_BASE_H

#include "core/math/color.h"
#include "core/variant/variant.h"

#include "../mm_node.h"

class GradientBase : public MMNode {
	GDCLASS(GradientBase, MMNode);

public:
	int get_interpolation_type() const;
	void set_interpolation_type(const int val);

	PoolRealArray get_points();
	void set_points(const PoolRealArray &val);

	Color get_gradient_color(const float x);
	virtual Color _get_gradient_color(const float x);

	float get_point_value(const int index);
	Color get_point_color(const int index);

	void add_point(const float val, const Color &color);

	int get_point_count();

	void clear();

	GradientBase();
	~GradientBase();

protected:
	static void _bind_methods();

	int interpolation_type;
	PoolRealArray points;
};

#endif
