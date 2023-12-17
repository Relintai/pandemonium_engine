#ifndef PROP_2D_LIGHT_H
#define PROP_2D_LIGHT_H


#include "core/math/color.h"
#include "core/object/reference.h"
#include "core/containers/vector.h"

class Prop2DLight : public Reference {
	GDCLASS(Prop2DLight, Reference);

public:
	Vector2 get_position();
	void set_position(const Vector2 &pos);

	Color get_color() const;
	void set_color(const Color &color);

	float get_size() const;
	void set_size(const float strength);

	Prop2DLight();
	~Prop2DLight();

private:
	static void _bind_methods();

private:
	Vector2 _position;

	Color _color;
	int _size;
};

#endif
