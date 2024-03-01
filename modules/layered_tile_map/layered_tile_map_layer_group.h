#ifndef TILE_MAP_LAYER_GROUP_H
#define TILE_MAP_LAYER_GROUP_H

/*************************************************************************/
/*  layered_tile_map_layer_group.h                                       */
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

#include "scene/2d/node_2d.h"

class TileSet;

class TileMapLayerGroup : public Node2D {
	GDCLASS(TileMapLayerGroup, Node2D);

private:
	mutable Vector<StringName> selected_layers;
	bool highlight_selected_layer = true;

#ifdef TOOLS_ENABLED
	void _cleanup_selected_layers();
#endif
	void _tile_set_changed();

protected:
	Ref<TileSet> tile_set;

	virtual void remove_child_notify(Node *p_child) override;

	static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
	// For editor use.
	void set_selected_layers(Vector<StringName> p_layer_names);
	Vector<StringName> get_selected_layers() const;
	void set_highlight_selected_layer(bool p_highlight_selected_layer);
	bool is_highlighting_selected_layer() const;
#endif

	// Accessors.
	void set_tileset(const Ref<TileSet> &p_tileset);
	Ref<TileSet> get_tileset() const;

	~TileMapLayerGroup();
};

#endif // TILE_MAP_LAYER_GROUP_H
