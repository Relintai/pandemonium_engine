/*************************************************************************/
/*  pscript_text_document.cpp                                           */
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

#include "pscript_text_document.h"

#include "../pscript.h"
#include "core/os/os.h"
#include "editor/editor_settings.h"
#include "editor_modules/editor_code_editor/editor_script_editor.h"
#include "pscript_extend_parser.h"
#include "pscript_language_protocol.h"

void PScriptTextDocument::_bind_methods() {
	ClassDB::bind_method(D_METHOD("didOpen"), &PScriptTextDocument::didOpen);
	ClassDB::bind_method(D_METHOD("didClose"), &PScriptTextDocument::didClose);
	ClassDB::bind_method(D_METHOD("didChange"), &PScriptTextDocument::didChange);
	ClassDB::bind_method(D_METHOD("didSave"), &PScriptTextDocument::didSave);
	ClassDB::bind_method(D_METHOD("nativeSymbol"), &PScriptTextDocument::nativeSymbol);
	ClassDB::bind_method(D_METHOD("documentSymbol"), &PScriptTextDocument::documentSymbol);
	ClassDB::bind_method(D_METHOD("completion"), &PScriptTextDocument::completion);
	ClassDB::bind_method(D_METHOD("resolve"), &PScriptTextDocument::resolve);
	ClassDB::bind_method(D_METHOD("rename"), &PScriptTextDocument::rename);
	ClassDB::bind_method(D_METHOD("foldingRange"), &PScriptTextDocument::foldingRange);
	ClassDB::bind_method(D_METHOD("codeLens"), &PScriptTextDocument::codeLens);
	ClassDB::bind_method(D_METHOD("documentLink"), &PScriptTextDocument::documentLink);
	ClassDB::bind_method(D_METHOD("colorPresentation"), &PScriptTextDocument::colorPresentation);
	ClassDB::bind_method(D_METHOD("hover"), &PScriptTextDocument::hover);
	ClassDB::bind_method(D_METHOD("definition"), &PScriptTextDocument::definition);
	ClassDB::bind_method(D_METHOD("declaration"), &PScriptTextDocument::declaration);
	ClassDB::bind_method(D_METHOD("signatureHelp"), &PScriptTextDocument::signatureHelp);
	ClassDB::bind_method(D_METHOD("show_native_symbol_in_editor"), &PScriptTextDocument::show_native_symbol_in_editor);
}

void PScriptTextDocument::didOpen(const Variant &p_param) {
	lsp::TextDocumentItem doc = load_document_item(p_param);
	sync_script_content(doc.uri, doc.text);
}

void PScriptTextDocument::didClose(const Variant &p_param) {
	// Left empty on purpose. Godot does nothing special on closing a document,
	// but it satisfies LSP clients that require didClose be implemented.
}

void PScriptTextDocument::didChange(const Variant &p_param) {
	lsp::TextDocumentItem doc = load_document_item(p_param);
	Dictionary dict = p_param;
	Array contentChanges = dict["contentChanges"];
	for (int i = 0; i < contentChanges.size(); ++i) {
		lsp::TextDocumentContentChangeEvent evt;
		evt.load(contentChanges[i]);
		doc.text = evt.text;
	}
	sync_script_content(doc.uri, doc.text);
}

void PScriptTextDocument::didSave(const Variant &p_param) {
	lsp::TextDocumentItem doc = load_document_item(p_param);
	Dictionary dict = p_param;
	String text = dict["text"];

	sync_script_content(doc.uri, text);
}

lsp::TextDocumentItem PScriptTextDocument::load_document_item(const Variant &p_param) {
	lsp::TextDocumentItem doc;
	Dictionary params = p_param;
	doc.load(params["textDocument"]);
	return doc;
}

void PScriptTextDocument::notify_client_show_symbol(const lsp::DocumentSymbol *symbol) {
	ERR_FAIL_NULL(symbol);
	PScriptLanguageProtocol::get_singleton()->notify_client("pscript/show_native_symbol", symbol->to_json(true));
}

void PScriptTextDocument::initialize() {
	if (PScriptLanguageProtocol::get_singleton()->is_smart_resolve_enabled()) {
		const HashMap<StringName, ClassMembers> &native_members = PScriptLanguageProtocol::get_singleton()->get_workspace()->native_members;

		const StringName *class_ptr = native_members.next(nullptr);
		while (class_ptr) {
			const ClassMembers &members = native_members.get(*class_ptr);

			const String *name = members.next(nullptr);
			while (name) {
				const lsp::DocumentSymbol *symbol = members.get(*name);
				lsp::CompletionItem item = symbol->make_completion_item();
				item.data = JOIN_SYMBOLS(String(*class_ptr), *name);
				native_member_completions.push_back(item.to_json());

				name = members.next(name);
			}

			class_ptr = native_members.next(class_ptr);
		}
	}
}

Variant PScriptTextDocument::nativeSymbol(const Dictionary &p_params) {
	Variant ret;

	lsp::NativeSymbolInspectParams params;
	params.load(p_params);

	if (const lsp::DocumentSymbol *symbol = PScriptLanguageProtocol::get_singleton()->get_workspace()->resolve_native_symbol(params)) {
		ret = symbol->to_json(true);
		notify_client_show_symbol(symbol);
	}

	return ret;
}

Array PScriptTextDocument::documentSymbol(const Dictionary &p_params) {
	Dictionary params = p_params["textDocument"];
	String uri = params["uri"];
	String path = PScriptLanguageProtocol::get_singleton()->get_workspace()->get_file_path(uri);
	Array arr;
	if (const RBMap<String, ExtendPScriptParser *>::Element *parser = PScriptLanguageProtocol::get_singleton()->get_workspace()->scripts.find(path)) {
		Vector<lsp::DocumentedSymbolInformation> list;
		parser->get()->get_symbols().symbol_tree_as_list(uri, list);
		for (int i = 0; i < list.size(); i++) {
			arr.push_back(list[i].to_json());
		}
	}
	return arr;
}

Array PScriptTextDocument::completion(const Dictionary &p_params) {
	Array arr;

	lsp::CompletionParams params;
	params.load(p_params);
	Dictionary request_data = params.to_json();

	List<ScriptCodeCompletionOption> options;
	PScriptLanguageProtocol::get_singleton()->get_workspace()->completion(params, &options);

	if (!options.empty()) {
		int i = 0;
		arr.resize(options.size());

		for (const List<ScriptCodeCompletionOption>::Element *E = options.front(); E; E = E->next()) {
			const ScriptCodeCompletionOption &option = E->get();
			lsp::CompletionItem item;
			item.label = option.display;
			item.data = request_data;
			item.insertText = option.insert_text;

			switch (option.kind) {
				case ScriptCodeCompletionOption::KIND_ENUM:
					item.kind = lsp::CompletionItemKind::Enum;
					break;
				case ScriptCodeCompletionOption::KIND_CLASS:
					item.kind = lsp::CompletionItemKind::Class;
					break;
				case ScriptCodeCompletionOption::KIND_MEMBER:
					item.kind = lsp::CompletionItemKind::Property;
					break;
				case ScriptCodeCompletionOption::KIND_FUNCTION:
					item.kind = lsp::CompletionItemKind::Method;
					break;
				case ScriptCodeCompletionOption::KIND_SIGNAL:
					item.kind = lsp::CompletionItemKind::Event;
					break;
				case ScriptCodeCompletionOption::KIND_CONSTANT:
					item.kind = lsp::CompletionItemKind::Constant;
					break;
				case ScriptCodeCompletionOption::KIND_VARIABLE:
					item.kind = lsp::CompletionItemKind::Variable;
					break;
				case ScriptCodeCompletionOption::KIND_FILE_PATH:
					item.kind = lsp::CompletionItemKind::File;
					break;
				case ScriptCodeCompletionOption::KIND_NODE_PATH:
					item.kind = lsp::CompletionItemKind::Snippet;
					break;
				case ScriptCodeCompletionOption::KIND_PLAIN_TEXT:
					item.kind = lsp::CompletionItemKind::Text;
					break;
			}

			arr[i] = item.to_json();
			i++;
		}
	} else if (PScriptLanguageProtocol::get_singleton()->is_smart_resolve_enabled()) {
		arr = native_member_completions.duplicate();

		for (RBMap<String, ExtendPScriptParser *>::Element *E = PScriptLanguageProtocol::get_singleton()->get_workspace()->scripts.front(); E; E = E->next()) {
			ExtendPScriptParser *script = E->get();
			const Array &items = script->get_member_completions();

			const int start_size = arr.size();
			arr.resize(start_size + items.size());
			for (int i = start_size; i < arr.size(); i++) {
				arr[i] = items[i - start_size];
			}
		}
	}
	return arr;
}

Dictionary PScriptTextDocument::rename(const Dictionary &p_params) {
	lsp::TextDocumentPositionParams params;
	params.load(p_params);
	String new_name = p_params["newName"];

	return PScriptLanguageProtocol::get_singleton()->get_workspace()->rename(params, new_name);
}

Dictionary PScriptTextDocument::resolve(const Dictionary &p_params) {
	lsp::CompletionItem item;
	item.load(p_params);

	lsp::CompletionParams params;
	Variant data = p_params["data"];

	const lsp::DocumentSymbol *symbol = nullptr;

	if (data.get_type() == Variant::DICTIONARY) {
		params.load(p_params["data"]);
		symbol = PScriptLanguageProtocol::get_singleton()->get_workspace()->resolve_symbol(params, item.label, item.kind == lsp::CompletionItemKind::Method || item.kind == lsp::CompletionItemKind::Function);

	} else if (data.get_type() == Variant::STRING) {
		String query = data;

		Vector<String> param_symbols = query.split(SYMBOL_SEPERATOR, false);

		if (param_symbols.size() >= 2) {
			String class_ = param_symbols[0];
			StringName class_name = class_;
			String member_name = param_symbols[param_symbols.size() - 1];
			String inner_class_name;
			if (param_symbols.size() >= 3) {
				inner_class_name = param_symbols[1];
			}

			if (const ClassMembers *members = PScriptLanguageProtocol::get_singleton()->get_workspace()->native_members.getptr(class_name)) {
				if (const lsp::DocumentSymbol *const *member = members->getptr(member_name)) {
					symbol = *member;
				}
			}

			if (!symbol) {
				if (const RBMap<String, ExtendPScriptParser *>::Element *E = PScriptLanguageProtocol::get_singleton()->get_workspace()->scripts.find(class_name)) {
					symbol = E->get()->get_member_symbol(member_name, inner_class_name);
				}
			}
		}
	}

	if (symbol) {
		item.documentation = symbol->render();
	}

	if (item.kind == lsp::CompletionItemKind::Event) {
		if (params.context.triggerKind == lsp::CompletionTriggerKind::TriggerCharacter && (params.context.triggerCharacter == "(")) {
			const String quote_style = EditorSettingsQuick::get_text_editor_completion_use_single_quotes() ? "'" : "\"";
			item.insertText = quote_style + item.label + quote_style;
		}
	}

	return item.to_json(true);
}

Array PScriptTextDocument::foldingRange(const Dictionary &p_params) {
	Array arr;
	return arr;
}

Array PScriptTextDocument::codeLens(const Dictionary &p_params) {
	Array arr;
	return arr;
}

Array PScriptTextDocument::documentLink(const Dictionary &p_params) {
	Array ret;

	lsp::DocumentLinkParams params;
	params.load(p_params);

	List<lsp::DocumentLink> links;
	PScriptLanguageProtocol::get_singleton()->get_workspace()->resolve_document_links(params.textDocument.uri, links);
	for (const List<lsp::DocumentLink>::Element *E = links.front(); E; E = E->next()) {
		ret.push_back(E->get().to_json());
	}
	return ret;
}

Array PScriptTextDocument::colorPresentation(const Dictionary &p_params) {
	Array arr;
	return arr;
}

Variant PScriptTextDocument::hover(const Dictionary &p_params) {
	lsp::TextDocumentPositionParams params;
	params.load(p_params);

	const lsp::DocumentSymbol *symbol = PScriptLanguageProtocol::get_singleton()->get_workspace()->resolve_symbol(params);
	if (symbol) {
		lsp::Hover hover;
		hover.contents = symbol->render();
		hover.range.start = params.position;
		hover.range.end = params.position;
		return hover.to_json();

	} else if (PScriptLanguageProtocol::get_singleton()->is_smart_resolve_enabled()) {
		Dictionary ret;
		Array contents;
		List<const lsp::DocumentSymbol *> list;
		PScriptLanguageProtocol::get_singleton()->get_workspace()->resolve_related_symbols(params, list);
		for (List<const lsp::DocumentSymbol *>::Element *E = list.front(); E; E = E->next()) {
			if (const lsp::DocumentSymbol *s = E->get()) {
				contents.push_back(s->render().value);
			}
		}
		ret["contents"] = contents;
		return ret;
	}

	return Variant();
}

Array PScriptTextDocument::definition(const Dictionary &p_params) {
	lsp::TextDocumentPositionParams params;
	params.load(p_params);
	List<const lsp::DocumentSymbol *> symbols;
	Array arr = this->find_symbols(params, symbols);
	return arr;
}

Variant PScriptTextDocument::declaration(const Dictionary &p_params) {
	lsp::TextDocumentPositionParams params;
	params.load(p_params);
	List<const lsp::DocumentSymbol *> symbols;
	Array arr = this->find_symbols(params, symbols);
	if (arr.empty() && !symbols.empty() && !symbols.front()->get()->native_class.empty()) { // Find a native symbol
		const lsp::DocumentSymbol *symbol = symbols.front()->get();
		if (PScriptLanguageProtocol::get_singleton()->is_goto_native_symbols_enabled()) {
			String id;
			switch (symbol->kind) {
				case lsp::SymbolKind::Class:
					id = "class_name:" + symbol->name;
					break;
				case lsp::SymbolKind::Constant:
					id = "class_constant:" + symbol->native_class + ":" + symbol->name;
					break;
				case lsp::SymbolKind::Property:
				case lsp::SymbolKind::Variable:
					id = "class_property:" + symbol->native_class + ":" + symbol->name;
					break;
				case lsp::SymbolKind::Enum:
					id = "class_enum:" + symbol->native_class + ":" + symbol->name;
					break;
				case lsp::SymbolKind::Method:
				case lsp::SymbolKind::Function:
					id = "class_method:" + symbol->native_class + ":" + symbol->name;
					break;
				default:
					id = "class_global:" + symbol->native_class + ":" + symbol->name;
					break;
			}
			call_deferred("show_native_symbol_in_editor", id);
		} else {
			notify_client_show_symbol(symbol);
		}
	}
	return arr;
}

Variant PScriptTextDocument::signatureHelp(const Dictionary &p_params) {
	Variant ret;

	lsp::TextDocumentPositionParams params;
	params.load(p_params);

	lsp::SignatureHelp s;
	if (OK == PScriptLanguageProtocol::get_singleton()->get_workspace()->resolve_signature(params, s)) {
		ret = s.to_json();
	}

	return ret;
}

PScriptTextDocument::PScriptTextDocument() {
	file_checker = FileAccess::create(FileAccess::ACCESS_RESOURCES);
}

PScriptTextDocument::~PScriptTextDocument() {
	memdelete(file_checker);
}

void PScriptTextDocument::sync_script_content(const String &p_path, const String &p_content) {
	String path = PScriptLanguageProtocol::get_singleton()->get_workspace()->get_file_path(p_path);
	PScriptLanguageProtocol::get_singleton()->get_workspace()->parse_script(path, p_content);

	EditorFileSystem::get_singleton()->upate_file(path);
	Error error;
	Ref<PScript> script = ResourceLoader::load(path, "", false, &error);
	if (error == OK) {
		if (script->load_source_code(path) == OK) {
			script->reload(true);
		}
	}
}

void PScriptTextDocument::show_native_symbol_in_editor(const String &p_symbol_id) {
	EditorScriptEditor::get_singleton()->call_deferred("_help_class_goto", p_symbol_id);
	OS::get_singleton()->move_window_to_foreground();
}

Array PScriptTextDocument::find_symbols(const lsp::TextDocumentPositionParams &p_location, List<const lsp::DocumentSymbol *> &r_list) {
	Array arr;
	const lsp::DocumentSymbol *symbol = PScriptLanguageProtocol::get_singleton()->get_workspace()->resolve_symbol(p_location);
	if (symbol) {
		lsp::Location location;
		location.uri = symbol->uri;
		location.range = symbol->range;
		const String &path = PScriptLanguageProtocol::get_singleton()->get_workspace()->get_file_path(symbol->uri);
		if (file_checker->file_exists(path)) {
			arr.push_back(location.to_json());
		}
		r_list.push_back(symbol);
	} else if (PScriptLanguageProtocol::get_singleton()->is_smart_resolve_enabled()) {
		List<const lsp::DocumentSymbol *> list;
		PScriptLanguageProtocol::get_singleton()->get_workspace()->resolve_related_symbols(p_location, list);
		for (List<const lsp::DocumentSymbol *>::Element *E = list.front(); E; E = E->next()) {
			if (const lsp::DocumentSymbol *s = E->get()) {
				if (!s->uri.empty()) {
					lsp::Location location;
					location.uri = s->uri;
					location.range = s->range;
					arr.push_back(location.to_json());
					r_list.push_back(s);
				}
			}
		}
	}
	return arr;
}
