/*************************************************************************/
/*  resource_importer_html_template_data.cpp                             */
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

#include "resource_importer_html_template_data.h"

#include "core/io/resource_saver.h"

#include "../html_template_data.h"

#ifdef MODULE_TEXT_EDITOR_ENABLED
#include "editor/editor_node.h"
#endif

String ResourceImporterHTMLTemplateData::get_importer_name() const {
	return "html_template_data";
}

String ResourceImporterHTMLTemplateData::get_visible_name() const {
	return "HTMLTemplateData";
}
void ResourceImporterHTMLTemplateData::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("phtpl");
}
String ResourceImporterHTMLTemplateData::get_save_extension() const {
	return "res";
}

String ResourceImporterHTMLTemplateData::get_resource_type() const {
	return "HTMLTemplateData";
}

bool ResourceImporterHTMLTemplateData::get_option_visibility(const String &p_option, const RBMap<StringName, Variant> &p_options) const {
	return true;
}

int ResourceImporterHTMLTemplateData::get_preset_count() const {
	return 0;
}
String ResourceImporterHTMLTemplateData::get_preset_name(int p_idx) const {
	return String();
}

void ResourceImporterHTMLTemplateData::get_import_options(List<ImportOption> *r_options, int p_preset) const {
}

Error ResourceImporterHTMLTemplateData::import(const String &p_source_file, const String &p_save_path, const RBMap<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files, Variant *r_metadata) {
	Ref<HTMLTemplateData> template_data;
	template_data.instance();
	
	Error err = template_data->load_from_file(p_source_file);
	
	if (err != OK) {
		return err;
	}

	return ResourceSaver::save(p_save_path + ".res", template_data);
}

ResourceImporterHTMLTemplateData::ResourceImporterHTMLTemplateData() {
}

ResourceImporterHTMLTemplateData::~ResourceImporterHTMLTemplateData() {
}

#ifdef MODULE_TEXT_EDITOR_ENABLED

void HTMLTemplateDataEditorPlugin::edit(Object *p_object) {
	Ref<HTMLTemplateData> f = Ref<HTMLTemplateData>(Object::cast_to<HTMLTemplateData>(p_object));
	
	if (f.is_valid()) {
		EditorPlugin *ep = EditorNode::get_singleton()->get_editor_by_name("Text");
		
		if (ep) {
			ep->call("open_file", f->get_path());
		}
	}
}

bool HTMLTemplateDataEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("HTMLTemplateData");
}

HTMLTemplateDataEditorPlugin::HTMLTemplateDataEditorPlugin(EditorNode *p_node) {
}

#endif
