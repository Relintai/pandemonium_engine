#ifndef PAGED_ARTICLES_WEB_PAGE_H
#define PAGED_ARTICLES_WEB_PAGE_H

/*************************************************************************/
/*  paged_articles_web_page.h                                            */
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

#include "core/containers/vector.h"
#include "core/string/ustring.h"

#include "../../http/web_node.h"

// This class will load and process all md files from the folder set to it's folder property,
// and generate one page from them. the files are processed in alphabetical order.

// The generated HTML looks like:

// render_menu()
// <div class="article_list_entry">
//	Contents of the first md file.
// </div>
// <div class="article_list_entry">
//	Contents of the second md file.
// </div>
// ...
// </div>

class WebServerRequest;

class PagedArticlesWebPage : public WebNode {
	GDCLASS(PagedArticlesWebPage, WebNode);

public:
	String get_folder();
	void set_folder(const String &val);

	void _handle_request(Ref<WebServerRequest> request);

	void _render_index(Ref<WebServerRequest> request);
	void _render_preview(Ref<WebServerRequest> request);

	void load();
	void generate_index_page();

	PagedArticlesWebPage();
	~PagedArticlesWebPage();

protected:
	void _notification(int what);

	static void _bind_methods();

	String _folder;

	String _index_page;
};

#endif
