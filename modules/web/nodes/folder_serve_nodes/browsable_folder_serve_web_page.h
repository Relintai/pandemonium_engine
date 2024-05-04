#ifndef BROWSABLE_FOLDER_SERVE_WEB_PAGE_H
#define BROWSABLE_FOLDER_SERVE_WEB_PAGE_H

/*************************************************************************/
/*  browsable_folder_serve_web_page.h                                    */
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

#include "core/containers/hash_map.h"
#include "core/string/ustring.h"

#include "folder_serve_web_page.h"

class WebServerRequest;

// On top of serving the files from the folder set to it's serve_folder property,
// this class also generates HTML directory lists. (Similar to apache's directory listing)
// It caches folder contents on ENTER_TREE, it does not watch for folder changes yet.

// if (should_render_menu) -> render_menu(request)
// <div class="file_list">
// <div class="file_list_entry"><a href="/files/">..</a></div>
// <div class="file_list_entry"><a href="/files/test_folder/">(Folder) test_folder</a></div>
// <div class="file_list_entry"><a href="/files/test_file.md">(File) test_file.md</a></div>
// ...
// </div>

class BrowsableFolderServeWebPage : public FolderServeWebPage {
	GDCLASS(BrowsableFolderServeWebPage, FolderServeWebPage);

public:
	bool get_should_render_menu();
	void set_should_render_menu(const bool &val);

	void _handle_request(Ref<WebServerRequest> request);

	void _render_index(Ref<WebServerRequest> request);
	void _render_preview(Ref<WebServerRequest> request);

	void load();

	void evaluate_dir(const String &path, const bool top_level = false);
	virtual void render_dir_page(const String &dir_uri, const Vector<String> &folders, const Vector<String> &files, const bool top_level);

	BrowsableFolderServeWebPage();
	~BrowsableFolderServeWebPage();

protected:
	static void _bind_methods();

	bool _should_render_menu;

	HashMap<String, String> _folder_indexes;
	String _index;
};

#endif
