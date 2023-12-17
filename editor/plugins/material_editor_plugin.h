#ifndef MATERIAL_EDITOR_PLUGIN_H
#define MATERIAL_EDITOR_PLUGIN_H

/*************************************************************************/
/*  material_editor_plugin.h                                             */
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

#include "editor/editor_inspector.h"
#include "editor/editor_plugin.h"
#include "editor/property_editor.h"
#include "scene/main/control.h"

#include "scene/resources/mesh/primitive_meshes.h"

#include "core/object/object.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "scene/resources/material/material.h"

#include "scene/resources/environment_3d.h"

class ViewportContainer;
class Camera;
class DirectionalLight;
class EditorNode;
class MeshInstance;
class Node;
class Resource;
class TextureButton;
class Viewport;

class MaterialEditor : public Control {
	GDCLASS(MaterialEditor, Control);

	Vector2 rot = Vector2();

	ViewportContainer *vc;
	Viewport *viewport;
	Spatial *rotation;
	MeshInstance *sphere_instance;
	MeshInstance *box_instance;
	DirectionalLight *light1;
	DirectionalLight *light2;
	Camera *camera;

	Ref<SphereMesh> sphere_mesh;
	Ref<CubeMesh> box_mesh;

	TextureButton *sphere_switch;
	TextureButton *box_switch;

	TextureButton *light_1_switch;
	TextureButton *light_2_switch;

	Ref<Material> material;

	void _button_pressed(Node *p_button);
	bool first_enter;

protected:
	void _notification(int p_what);
	void _gui_input(const Ref<InputEvent> &p_event);
	void _update_rotation();

	static void _bind_methods();

public:
	void edit(Ref<Material> p_material, const Ref<Environment3D> &p_env);
	MaterialEditor();
	~MaterialEditor();
};

class EditorInspectorPluginMaterial : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginMaterial, EditorInspectorPlugin);
	Ref<Environment3D> env;

public:
	virtual bool can_handle(Object *p_object);
	virtual void parse_begin(Object *p_object);

	EditorInspectorPluginMaterial();
};

class MaterialEditorPlugin : public EditorPlugin {
	GDCLASS(MaterialEditorPlugin, EditorPlugin);

public:
	virtual String get_name() const { return "Material"; }

	MaterialEditorPlugin(EditorNode *p_node);
};

class SpatialMaterialConversionPlugin : public EditorResourceConversionPlugin {
	GDCLASS(SpatialMaterialConversionPlugin, EditorResourceConversionPlugin);

public:
	virtual String converts_to() const;
	virtual bool handles(const Ref<Resource> &p_resource) const;
	virtual Ref<Resource> convert(const Ref<Resource> &p_resource) const;
};

class ParticlesMaterialConversionPlugin : public EditorResourceConversionPlugin {
	GDCLASS(ParticlesMaterialConversionPlugin, EditorResourceConversionPlugin);

public:
	virtual String converts_to() const;
	virtual bool handles(const Ref<Resource> &p_resource) const;
	virtual Ref<Resource> convert(const Ref<Resource> &p_resource) const;
};

class CanvasItemMaterialConversionPlugin : public EditorResourceConversionPlugin {
	GDCLASS(CanvasItemMaterialConversionPlugin, EditorResourceConversionPlugin);

public:
	virtual String converts_to() const;
	virtual bool handles(const Ref<Resource> &p_resource) const;
	virtual Ref<Resource> convert(const Ref<Resource> &p_resource) const;
};

#endif // MATERIAL_EDITOR_PLUGIN_H
