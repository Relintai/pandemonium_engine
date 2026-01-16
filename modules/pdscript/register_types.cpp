/*************************************************************************/
/*  register_types.cpp                                                   */
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

#include "register_types.h"

#include "core/io/file_access_encrypted.h"
#include "core/io/resource_loader.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "pdscript.h"
#include "pdscript_tokenizer.h"

#include "modules/modules_enabled.gen.h"

PDScriptLanguage *script_language_pd = nullptr;
Ref<ResourceFormatLoaderPDScript> resource_loader_pd;
Ref<ResourceFormatSaverPDScript> resource_saver_pd;

#ifdef TOOLS_ENABLED

#include "editor/editor_export.h"
#include "editor/editor_node.h"
#include "editor/editor_settings.h"
#include "editor/pdscript_highlighter.h"

#ifdef MODULE_EDITOR_CODE_EDITOR_ENABLED
#include "editor_code_editor/editor_script_editor.h"
#endif

#ifndef PDSCRIPT_NO_LSP
#include "core/config/engine.h"
#include "language_server/pdscript_language_server.h"
#endif // !PDSCRIPT_NO_LSP

class EditorExportPDScript : public EditorExportPlugin {
	GDCLASS(EditorExportPDScript, EditorExportPlugin);

public:
	virtual void _export_file(const String &p_path, const String &p_type, const RBSet<String> &p_features) {
		int script_mode = EditorExportPreset::MODE_SCRIPT_COMPILED;
		String script_key;

		const Ref<EditorExportPreset> &preset = get_export_preset();

		if (preset.is_valid()) {
			script_mode = preset->get_script_export_mode();
			script_key = preset->get_script_encryption_key().to_lower();
		}

		if (!p_path.ends_with(".pd") || script_mode == EditorExportPreset::MODE_SCRIPT_TEXT) {
			return;
		}

		Vector<uint8_t> file = FileAccess::get_file_as_array(p_path);
		if (file.empty()) {
			return;
		}

		String txt;
		txt.parse_utf8((const char *)file.ptr(), file.size());
		file = PDScriptTokenizerBuffer::parse_code_string(txt);

		if (!file.empty()) {
			if (script_mode == EditorExportPreset::MODE_SCRIPT_ENCRYPTED) {
				String tmp_path = EditorSettings::get_singleton()->get_cache_dir().plus_file("script.pde");
				FileAccess *fa = FileAccess::open(tmp_path, FileAccess::WRITE);

				Vector<uint8_t> key;
				key.resize(32);
				for (int i = 0; i < 32; i++) {
					int v = 0;
					if (i * 2 < script_key.length()) {
						CharType ct = script_key[i * 2];
						if (ct >= '0' && ct <= '9') {
							ct = ct - '0';
						} else if (ct >= 'a' && ct <= 'f') {
							ct = 10 + ct - 'a';
						}
						v |= ct << 4;
					}

					if (i * 2 + 1 < script_key.length()) {
						CharType ct = script_key[i * 2 + 1];
						if (ct >= '0' && ct <= '9') {
							ct = ct - '0';
						} else if (ct >= 'a' && ct <= 'f') {
							ct = 10 + ct - 'a';
						}
						v |= ct;
					}
					key.write[i] = v;
				}
				FileAccessEncrypted *fae = memnew(FileAccessEncrypted);
				Error err = fae->open_and_parse(fa, key, FileAccessEncrypted::MODE_WRITE_AES256);

				if (err == OK) {
					fae->store_buffer(file.ptr(), file.size());
				}

				memdelete(fae);

				file = FileAccess::get_file_as_array(tmp_path);
				add_file(p_path.get_basename() + ".pde", file, true);

				// Clean up temporary file.
				DirAccess::remove_file_or_error(tmp_path);

			} else {
				add_file(p_path.get_basename() + ".pdc", file, true);
			}
		}
	}
};

static void _editor_init() {
	Ref<EditorExportPDScript> pd_export;
	pd_export.instance();
	EditorExport::get_singleton()->add_export_plugin(pd_export);

#ifdef MODULE_EDITOR_CODE_EDITOR_ENABLED
	Ref<PDScriptSyntaxHighlighter> pdscript_syntax_highlighter;
	pdscript_syntax_highlighter.instance();
	EditorScriptEditor::get_singleton()->register_syntax_highlighter(pdscript_syntax_highlighter);
#endif

#ifndef PDSCRIPT_NO_LSP
	register_pd_lsp_types();
	PDScriptLanguageServer *lsp_plugin = memnew(PDScriptLanguageServer);
	EditorNode::get_singleton()->add_editor_plugin(lsp_plugin);
	Engine::get_singleton()->add_singleton(Engine::Singleton("PDScriptLanguageProtocol", PDScriptLanguageProtocol::get_singleton()));
#endif // !PDSCRIPT_NO_LSP
}

#endif // TOOLS_ENABLED

void register_pdscript_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		script_language_pd = memnew(PDScriptLanguage);
		ScriptServer::register_language(script_language_pd);

		resource_loader_pd.instance();
		ResourceLoader::add_resource_format_loader(resource_loader_pd);

		resource_saver_pd.instance();
		ResourceSaver::add_resource_format_saver(resource_saver_pd);
	}

	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<PDScript>();
		ClassDB::register_virtual_class<PDScriptFunctionState>();
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_REGISTRATION_LEVEL_EDITOR) {
		EditorNode::add_init_callback(_editor_init);
	}
#endif // TOOLS_ENABLED
}

void unregister_pdscript_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		ScriptServer::unregister_language(script_language_pd);

		if (script_language_pd) {
			memdelete(script_language_pd);
		}

		ResourceLoader::remove_resource_format_loader(resource_loader_pd);
		resource_loader_pd.unref();

		ResourceSaver::remove_resource_format_saver(resource_saver_pd);
		resource_saver_pd.unref();
	}
}
