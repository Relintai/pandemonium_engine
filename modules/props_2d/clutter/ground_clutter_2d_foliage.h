#ifndef GROUND_CLUTTER_FOLIAGE_2D_H
#define GROUND_CLUTTER_FOLIAGE_2D_H


#include "core/containers/vector.h"

#include "ground_clutter_2d.h"

#include "scene/resources/texture.h"

class GroundClutter2DFoliage : public GroundClutter2D {
	GDCLASS(GroundClutter2DFoliage, GroundClutter2D);

public:
	int get_texture_count() const;
	Ref<Texture> get_texture(const int index);
	void remove_texture(const int index);
	void add_texture(Ref<Texture> texture);

	GroundClutter2DFoliage();
	~GroundClutter2DFoliage();

private:
	static void _bind_methods();

private:
	Vector<Ref<Texture>> _textures;
};

#endif
