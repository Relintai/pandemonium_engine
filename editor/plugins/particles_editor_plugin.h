#ifndef PARTICLES_EDITOR_PLUGIN_H
#define PARTICLES_EDITOR_PLUGIN_H
/*************************************************************************/
/*  particles_editor_plugin.h                                            */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "scene/main/control.h"
#include "editor/editor_plugin.h"

#include "core/math/face3.h"
#include "core/string/node_path.h"
#include "core/object/object.h"
#include "core/containers/pool_vector.h"


class ConfirmationDialog;
class EditorFileDialog;
class HBoxContainer;
class MenuButton;
class OptionButton;
class Panel;
class SceneTreeDialog;
class Spatial;
class SpinBox;
struct Vector3;

class ParticlesEditorBase : public Control {
	GDCLASS(ParticlesEditorBase, Control);

protected:
	Spatial *base_node;
	Panel *panel;
	MenuButton *options;
	HBoxContainer *particles_editor_hb;

	EditorFileDialog *emission_file_dialog;
	SceneTreeDialog *emission_tree_dialog;

	ConfirmationDialog *emission_dialog;
	SpinBox *emission_amount;
	OptionButton *emission_fill;

	PoolVector<Face3> geometry;

	bool _generate(PoolVector<Vector3> &points, PoolVector<Vector3> &normals);
	virtual void _generate_emission_points() = 0;
	void _node_selected(const NodePath &p_path);

	static void _bind_methods();

public:
	ParticlesEditorBase();
};

#endif // PARTICLES_EDITOR_PLUGIN_H
