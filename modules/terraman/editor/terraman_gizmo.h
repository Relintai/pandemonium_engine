#ifndef TERRAMAN_GIZMO_H
#define TERRAMAN_GIZMO_H

/*************************************************************************/
/*  terraman_gizmo.h                                                     */
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

#include "editor/spatial_editor_gizmos.h"

#include "core/variant/variant.h"

class Camera;
class MeshDataResource;
class MDREDMeshOutline;
class InputEvent;
class EditorPlugin;
class UndoRedo;

class TerramanGizmo : public EditorSpatialGizmo {
	GDCLASS(TerramanGizmo, EditorSpatialGizmo);

public:
	void set_visible(const bool visible);

	void set_editor_plugin(EditorPlugin *editor_plugin);

	void set_handle(int index, bool secondary, Camera *camera, const Point2 &point);
	void redraw();
	void apply();

	TerramanGizmo();
	~TerramanGizmo();

	PoolVector3Array _vertices;
	PoolIntArray _indices;

	EditorPlugin *_editor_plugin;
	UndoRedo *_undo_redo;

	bool _visible;

protected:
	static void _bind_methods();
};

#endif
