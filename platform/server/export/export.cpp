/*************************************************************************/
/*  export.cpp                                                           */
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

#include "export.h"

#include "core/os/file_access.h"
#include "editor/editor_export.h"
#include "platform/server/logo.gen.h"
#include "scene/resources/texture.h"

void register_server_exporter() {
	Ref<Image> img = memnew(Image(_server_logo));
	Ref<ImageTexture> logo;
	logo.instance();
	logo->create_from_image(img);

	Ref<EditorExportPlatformPC> platform_linux_server;
	platform_linux_server.instance();
	platform_linux_server->set_logo(logo);
	platform_linux_server->set_name("Linux/Server");
	platform_linux_server->set_extension("x86_64", "x86_64");
	platform_linux_server->set_extension("x86", "x86_32");
	platform_linux_server->set_extension("arm64", "arm64");
	platform_linux_server->set_extension("arm32", "arm32");
	platform_linux_server->set_release_files("x86_64", "linux_server_64_release");
	platform_linux_server->set_release_files("x86_32", "linux_server_32_release");
	platform_linux_server->set_release_files("arm64", "linux_server_arm64_release");
	platform_linux_server->set_release_files("arm32", "linux_server_arm32_release");
	platform_linux_server->set_debug_files("x86_64", "linux_server_64_debug");
	platform_linux_server->set_debug_files("x86_32", "linux_server_32_debug");
	platform_linux_server->set_debug_files("arm64", "linux_server_arm64_debug");
	platform_linux_server->set_debug_files("arm32", "linux_server_arm32_debug");
	platform_linux_server->set_os_name("X11");
	platform_linux_server->set_chmod_flags(0755);

	Ref<EditorExportPlatformPC> platform_linux_http_server;
	platform_linux_http_server.instance();
	platform_linux_http_server->set_logo(logo);
	platform_linux_http_server->set_name("Linux/HTTPServer");
	platform_linux_http_server->set_extension("x86_64", "x86_64");
	platform_linux_http_server->set_extension("x86", "x86_32");
	platform_linux_http_server->set_extension("arm64", "arm64");
	platform_linux_http_server->set_extension("arm32", "arm32");
	platform_linux_http_server->set_release_files("x86_64", "linux_http_server_64_release");
	platform_linux_http_server->set_release_files("x86_32", "linux_http_server_32_release");
	platform_linux_http_server->set_release_files("arm64", "linux_http_server_arm64_release");
	platform_linux_http_server->set_release_files("arm32", "linux_http_server_arm32_release");
	platform_linux_http_server->set_debug_files("x86_64", "linux_http_server_64_debug");
	platform_linux_http_server->set_debug_files("x86_32", "linux_http_server_32_debug");
	platform_linux_http_server->set_debug_files("arm64", "linux_http_server_arm64_debug");
	platform_linux_http_server->set_debug_files("arm32", "linux_http_server_arm32_debug");
	platform_linux_http_server->set_os_name("X11");
	platform_linux_http_server->set_chmod_flags(0755);

	// TODO the server platform can be built on bsd, and OSX. Add those eventually aswell.

	EditorExport::get_singleton()->add_export_platform(platform_linux_server);
	EditorExport::get_singleton()->add_export_platform(platform_linux_http_server);
}
