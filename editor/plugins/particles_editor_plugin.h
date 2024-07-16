#ifndef PARTICLES_EDITOR_PLUGIN_H
#define PARTICLES_EDITOR_PLUGIN_H

/*************************************************************************/
/*  particles_editor_plugin.h                                            */
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

#include "editor/editor_plugin.h"
#include "scene/main/control.h"

#include "core/containers/pool_vector.h"
#include "core/math/face3.h"
#include "core/object/object.h"
#include "core/string/node_path.h"

class ConfirmationDialog;
class EditorFileDialog;
class HBoxContainer;
class MenuButton;
class OptionButton;
class Panel;
class SceneTreeDialog;
class Spatial;
class SpinBox;
class GPUParticles;
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

class GPUParticlesEditor : public ParticlesEditorBase {
	GDCLASS(GPUParticlesEditor, ParticlesEditorBase);

	ConfirmationDialog *generate_aabb;
	SpinBox *generate_seconds;
	GPUParticles *node;

	enum Menu {

		MENU_OPTION_GENERATE_AABB,
		MENU_OPTION_CREATE_EMISSION_VOLUME_FROM_NODE,
		MENU_OPTION_CREATE_EMISSION_VOLUME_FROM_MESH,
		MENU_OPTION_CLEAR_EMISSION_VOLUME,
		MENU_OPTION_CONVERT_TO_CPU_PARTICLES,
		MENU_OPTION_RESTART,

	};

	void _generate_aabb();

	void _menu_option(int);

	friend class GPUParticlesEditorPlugin;

	virtual void _generate_emission_points();

protected:
	void _notification(int p_notification);
	void _node_removed(Node *p_node);
	static void _bind_methods();

public:
	void edit(GPUParticles *p_particles);
	GPUParticlesEditor();
};

class GPUParticlesEditorPlugin : public EditorPlugin {
	GDCLASS(GPUParticlesEditorPlugin, EditorPlugin);

	GPUParticlesEditor *particles_editor;
	EditorNode *editor;

public:
	virtual String get_name() const { return "GPUParticles"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;
	virtual void make_visible(bool p_visible);

	GPUParticlesEditorPlugin(EditorNode *p_node);
	~GPUParticlesEditorPlugin();
};


#endif // PARTICLES_EDITOR_PLUGIN_H
