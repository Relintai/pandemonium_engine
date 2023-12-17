#ifndef TILED_WALL_2D_H
#define TILED_WALL_2D_H

/*************************************************************************/
/*  tiled_wall_2d.h                                                      */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

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
