/*
Copyright (c) 2019-2023 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "editor_import_gltf_mdr.h"

String EditorImportGLTFMdr::get_importer_name() const {
	return "gltf_mdr";
}

String EditorImportGLTFMdr::get_visible_name() const {
	return "GLTF MDR";
}

void EditorImportGLTFMdr::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("gltf");
	p_extensions->push_back("glb");
}

String EditorImportGLTFMdr::get_save_extension() const {
	return "res";
}

String EditorImportGLTFMdr::get_resource_type() const {
	return "MeshDataResource";
}

float EditorImportGLTFMdr::get_priority() const {
	return 1.0;
}

int EditorImportGLTFMdr::get_preset_count() const {
	return 0;
}

String EditorImportGLTFMdr::get_preset_name(int p_idx) const {
	return "";
}

Error EditorImportGLTFMdr::import(const String &p_source_file, const String &p_save_path, const Map<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files, Variant *r_metadata) {
	//MeshDataResource::ColliderType collider_type = static_cast<MeshDataResource::ColliderType>(static_cast<int>(p_options["collider_type"]));

	Error erri;

	Node *n = _importer->import_scene(p_source_file, 0, 15, 0, nullptr, &erri);

	ERR_FAIL_COND_V(!n, Error::ERR_PARSE_ERROR);

	if (erri != Error::OK) {
		return erri;
	}

	Error err = process_node(n, p_source_file, p_save_path, p_options, r_platform_variants, r_gen_files, r_metadata);

	n->queue_delete();
	return err;
}

EditorImportGLTFMdr::EditorImportGLTFMdr() {
	_importer.instance();
}

EditorImportGLTFMdr::~EditorImportGLTFMdr() {
	_importer.unref();
}
