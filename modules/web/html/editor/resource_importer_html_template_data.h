#ifndef RESOURCE_IMPORTER_HTML_TEMPLATE_DATA_H
#define RESOURCE_IMPORTER_HTML_TEMPLATE_DATA_H

/*************************************************************************/
/*  resource_importer_html_template_data.h                               */
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

#include "core/io/resource_importer.h"

#include "editor/editor_plugin.h"
#include "modules/modules_enabled.gen.h"

class EditorNode;

class ResourceImporterHTMLTemplateData : public ResourceImporter {
	GDCLASS(ResourceImporterHTMLTemplateData, ResourceImporter);

public:
	virtual String get_importer_name() const;
	virtual String get_visible_name() const;
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual String get_save_extension() const;
	virtual String get_resource_type() const;

	virtual int get_preset_count() const;
	virtual String get_preset_name(int p_idx) const;

	virtual void get_import_options(List<ImportOption> *r_options, int p_preset = 0) const;
	virtual bool get_option_visibility(const String &p_option, const RBMap<StringName, Variant> &p_options) const;
	virtual Error import(const String &p_source_file, const String &p_save_path, const RBMap<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files = nullptr, Variant *r_metadata = nullptr);

	ResourceImporterHTMLTemplateData();
	~ResourceImporterHTMLTemplateData();
};

#ifdef MODULE_TEXT_EDITOR_ENABLED

class HTMLTemplateDataEditorPlugin : public EditorPlugin {
	GDCLASS(HTMLTemplateDataEditorPlugin, EditorPlugin);

public:
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;
	
	HTMLTemplateDataEditorPlugin(EditorNode *p_node);

	virtual String get_name() const { return "HTMLTemplateData"; }
};

#endif

#endif // RESOURCE_IMPORTER_HTML_TEMPLATE_DATA_H
