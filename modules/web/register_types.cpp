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

//#include "core/config/engine.h"

#include "file_cache.h"

#include "html/bbcode_parser.h"
#include "html/form_validator.h"
#include "html/html_builder_bind.h"
#include "html/html_parser.h"
#include "html/html_template.h"
#include "html/html_template_multilang.h"
#include "html/html_template_data.h"
#include "html/markdown_renderer.h"
#include "html/paginator.h"

#if TOOLS_ENABLED
#include "html/editor/resource_importer_html_template_data.h"
#endif

#include "http/csrf_token.h"
#include "http/http_server_enums.h"
#include "http/http_session.h"
#include "http/http_session_manager.h"
#include "http/http_session_manager_file.h"

#include "http/web_node.h"
#include "http/web_permission.h"
#include "http/web_root.h"
#include "http/web_server.h"
#include "http/web_server_cookie.h"
#include "http/web_server_middleware.h"
#include "http/web_server_request.h"
#include "http/web_server_request_scriptable.h"

#include "nodes/static_pages/static_web_page.h"
#include "nodes/static_pages/static_web_page_file.h"
#include "nodes/static_pages/static_web_page_folder_files.h"

#include "nodes/folder_serve_nodes/browsable_folder_serve_web_page.h"
#include "nodes/folder_serve_nodes/folder_serve_web_page.h"

#include "nodes/list_page/list_web_page.h"

#include "nodes/paged_article/paged_article_web_page.h"
#include "nodes/paged_article/paged_articles_web_page.h"
//#include "nodes/paged_article/paged_articles_md_index.h"

#include "nodes/redirect/alias_web_page.h"
#include "nodes/redirect/redirect_web_page.h"

#if TOOLS_ENABLED
#include "editor/web_node_editor_plugin.h"
#endif

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_DATABASE_ENABLED
#include "database/http_session_manager_db.h"
#endif

void register_web_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<_HTMLBuilder>();
		ClassDB::register_class<_HTMLTag>();

		ClassDB::register_class<HTMLTemplate>();
		ClassDB::register_class<HTMLTemplateMultilang>();
		ClassDB::register_class<HTMLTemplateData>();

		ClassDB::register_class<HTMLPaginator>();

		ClassDB::register_class<FormFieldEntry>();
		ClassDB::register_class<FormExistsFieldEntry>();
		ClassDB::register_class<FormIntFieldEntry>();
		ClassDB::register_class<FormFloatFieldEntry>();
		ClassDB::register_class<FormAlphaFieldEntry>();
		ClassDB::register_class<FormAlphaNumericFieldEntry>();
		ClassDB::register_class<FormNeedsLowercaseCharacterFieldEntry>();
		ClassDB::register_class<FormNeedsUppercaseCharacterFieldEntry>();
		ClassDB::register_class<FormNeedsOtherCharacterFieldEntry>();
		ClassDB::register_class<FormMinimumLengthFieldEntry>();
		ClassDB::register_class<FormMaximumLengthFieldEntry>();
		ClassDB::register_class<FormEmailFieldEntry>();
		ClassDB::register_class<FormNeedToMatchOtherFieldEntry>();
		ClassDB::register_class<FormField>();
		ClassDB::register_class<FormValidator>();

		ClassDB::register_class<HTMLParserAttribute>();
		ClassDB::register_class<HTMLParserTag>();
		ClassDB::register_class<HTMLParser>();

		ClassDB::register_class<BBCodeParserAttribute>();
		ClassDB::register_class<BBCodeParserTag>();
		ClassDB::register_class<BBCodeParser>();

		ClassDB::register_class<MarkdownRenderer>();
		ClassDB::register_class<MarkdownRendererCustomRendererCallback>();

		ClassDB::register_class<FileCache>();

		ClassDB::register_class<HTTPServerEnums>();

		ClassDB::register_class<HTTPSession>();
		ClassDB::register_class<HTTPSessionManager>();
		ClassDB::register_class<HTTPSessionManagerFile>();
		ClassDB::register_class<SessionSetupWebServerMiddleware>();
		
		ClassDB::register_class<WebNode>();
		ClassDB::register_class<WebPermission>();
		ClassDB::register_class<WebRoot>();
		ClassDB::register_class<WebServer>();
		ClassDB::register_class<WebServerCookie>();
		ClassDB::register_class<WebServerMiddleware>();
		ClassDB::register_class<WebServerRequest>();
		ClassDB::register_class<WebServerRequestScriptable>();

		ClassDB::register_class<CSRFTokenWebServerMiddleware>();
		ClassDB::register_class<LocaleSetupWebServerMiddleware>();

		ClassDB::register_class<StaticWebPage>();
		ClassDB::register_class<StaticWebPageFile>();
		ClassDB::register_class<StaticWebPageFolderFiles>();

		ClassDB::register_class<FolderServeWebPage>();
		ClassDB::register_class<BrowsableFolderServeWebPage>();

		ClassDB::register_class<ListWebPage>();

		ClassDB::register_class<PagedArticleWebPage>();
		ClassDB::register_class<PagedArticlesWebPage>();
		//ClassDB::register_class<PagedArticlesWebPageMDIndex>();

		ClassDB::register_class<RedirectWebPage>();
		ClassDB::register_class<AliasWebPage>();

#ifdef MODULE_DATABASE_ENABLED
		ClassDB::register_class<HTTPSessionManagerDB>();
#endif
	}

#if TOOLS_ENABLED
	if (p_level == MODULE_REGISTRATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<WebNodeEditorPlugin>();

#ifdef MODULE_TEXT_EDITOR_ENABLED
		EditorPlugins::add_by_type<HTMLTemplateDataEditorPlugin>();
#endif

		Ref<ResourceImporterHTMLTemplateData> html_template_data_importer;
		html_template_data_importer.instance();
		ResourceFormatImporter::get_singleton()->add_importer(html_template_data_importer);
	}
#endif
}

void unregister_web_types(ModuleRegistrationLevel p_level) {
}
