/*************************************************************************/
/*  importer_mesh_instance_3d.h                                          */
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

#ifndef IMPORTER_MESH_INSTANCE_3D_H
#define IMPORTER_MESH_INSTANCE_3D_H

#include "scene/main/spatial.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_SKELETON_3D_ENABLED
#include "modules/skeleton_3d/resources/skin.h"
#endif

class ImporterMesh;

class ImporterMeshInstance3D : public Spatial {
	GDCLASS(ImporterMeshInstance3D, Spatial)

	Ref<ImporterMesh> mesh;
#ifdef MODULE_SKELETON_3D_ENABLED
	Ref<Skin> skin;
	NodePath skeleton_path;
#endif
	Vector<Ref<Material>> surface_materials;

protected:
	static void _bind_methods();

public:
	void set_mesh(const Ref<ImporterMesh> &p_mesh);
	Ref<ImporterMesh> get_mesh() const;

#ifdef MODULE_SKELETON_3D_ENABLED
	void set_skin(const Ref<Skin> &p_skin);
	Ref<Skin> get_skin() const;
#endif

	void set_surface_material(int p_idx, const Ref<Material> &p_material);
	Ref<Material> get_surface_material(int p_idx) const;

#ifdef MODULE_SKELETON_3D_ENABLED
	void set_skeleton_path(const NodePath &p_path);
	NodePath get_skeleton_path() const;
#endif
};

#endif // IMPORTER_MESH_INSTANCE_3D_H
