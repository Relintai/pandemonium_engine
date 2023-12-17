#ifndef TERRAIN_2D_SURFACE_H
#define TERRAIN_2D_SURFACE_H


#include "core/math/color.h"
#include "core/object/resource.h"
#include "core/containers/vector.h"

#include "core/math/rect2.h"
#include "scene/resources/material/material.h"

#include "terrain_2d_library.h"

class Terrain2DLibrary;

class Terrain2DSurface : public Resource {
	GDCLASS(Terrain2DSurface, Resource)

public:
	int get_id() const;
	void set_id(const int value);

	Rect2 get_rect() const;
	void set_rect(const Rect2 &rect);

	Ref<Terrain2DLibrary> get_library() const;
	void set_library(Ref<Terrain2DLibrary> library);

	Vector2 transform_uv(const Vector2 &p_uv) const;
	Vector2 transform_uv_scaled(const Vector2 &p_uv, const int p_current_x, const int p_current_y, const int p_max) const;

	virtual void refresh_rects();

	Terrain2DSurface();
	~Terrain2DSurface();

protected:
	static void _bind_methods();

	Terrain2DLibrary *_library;

	int _id;
	int _mesher_index;
	bool _transparent;
	bool _liquid;
	Rect2 _rect;
};

#endif
