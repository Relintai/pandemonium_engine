#ifndef PROP_LIGHT_H
#define PROP_LIGHT_H


#include "core/math/color.h"
#include "core/object/reference.h"
#include "core/containers/vector.h"

class PropLight : public Reference {
	GDCLASS(PropLight, Reference);

public:
	Vector3 get_position();
	void set_position(const Vector3 &pos);

	Color get_color() const;
	void set_color(const Color &color);

	float get_size() const;
	void set_size(const float strength);

	PropLight();
	~PropLight();

private:
	static void _bind_methods();

private:
	Vector3 _position;

	Color _color;
	int _size;
};

#endif
