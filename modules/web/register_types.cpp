/*
Copyright (c) 2022 Péter Magyar

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

#include "register_types.h"

//#include "core/engine.h"

#include "file_cache.h"

#include "html/bbcode_parser.h"
#include "html/form_validator.h"
#include "html/html_builder_bind.h"
#include "html/html_parser.h"
#include "html/markdown_renderer.h"
#include "html/paginator.h"

#include "http/csrf_token.h"
#include "http/http_server_enums.h"
#include "http/http_session.h"
#include "http/http_session_manager.h"
#include "http/web_node.h"
#include "http/web_permission.h"
#include "http/web_root.h"
#include "http/web_server.h"
#include "http/web_server_cookie.h"
#include "http/web_server_middleware.h"
#include "http/web_server_request.h"

#include "http_server_simple/web_server_simple.h"

#include "nodes/static_pages/static_web_page.h"
#include "nodes/static_pages/static_web_page_file.h"
#include "nodes/static_pages/static_web_page_folder_files.h"

#include "nodes/folder_serve_nodes/browsable_folder_serve_web_page.h"
#include "nodes/folder_serve_nodes/folder_serve_web_page.h"

#include "nodes/list_page/list_web_page.h"

#include "nodes/paged_article/paged_article_web_page.h"
#include "nodes/paged_article/paged_articles_web_page.h"
//#include "nodes/paged_article/paged_articles_md_index.h"

#include "nodes/redirect/redirect_web_page.h"

void register_web_types() {
	ClassDB::register_class<_HTMLBuilder>();
	ClassDB::register_class<_HTMLTag>();

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

	ClassDB::register_class<FileCache>();

	ClassDB::register_class<HTTPServerEnums>();

	ClassDB::register_class<CSRFTokenWebServerMiddleware>();
	ClassDB::register_class<HTTPSession>();
	ClassDB::register_class<HTTPSessionManager>();
	ClassDB::register_class<SessionSetupWebServerMiddleware>();
	ClassDB::register_class<WebNode>();
	ClassDB::register_class<WebPermission>();
	ClassDB::register_class<WebRoot>();
	ClassDB::register_class<WebServer>();
	ClassDB::register_class<WebServerCookie>();
	ClassDB::register_class<WebServerMiddleware>();
	ClassDB::register_class<WebServerRequest>();

	ClassDB::register_class<WebServerSimple>();

	ClassDB::register_class<StaticPage>();
	ClassDB::register_class<StaticPageFile>();
	ClassDB::register_class<StaticPageFolderFiles>();

	ClassDB::register_class<FolderServeNode>();
	ClassDB::register_class<BrowsableFolderServeNode>();

	ClassDB::register_class<ListPage>();

	ClassDB::register_class<PagedArticle>();
	ClassDB::register_class<PagedArticles>();
	//ClassDB::register_class<PagedArticlesMDIndex>();

	ClassDB::register_class<RedirectWebPage>();
}

void unregister_web_types() {
}
