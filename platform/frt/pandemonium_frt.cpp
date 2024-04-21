/*************************************************************************/
/*  pandemonium_frt.cpp                                                  */
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

// pandemonium_frt.cpp
/*
 * FRT - A Godot platform targeting single board computers
 * Copyright (c) 2017-2019  Emanuele Fornara
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "main/main.h"

#include "frt.h"

#include "bits/frt_app_impl.h"

using namespace frt;

static Env env;
static Runnable *runnable;

static void probe_modules_fallback() {
	App *app = App::instance();
	Module *module;

	const char *video_modules[] = {
		"video_x11",
		"video_fbdev",
		"video_bcm",
		"video_kmsdrm",
		0
	};
	if (!env.video) {
		module = app->probe(video_modules);
		env.video = (Video *)module;
	}

	const char *keyboard_modules[] = {
		"keyboard_x11",
		"keyboard_linux_input",
		0
	};
	if (!env.keyboard) {
		module = app->probe(keyboard_modules);
		env.keyboard = (Keyboard *)module;
	}

	const char *mouse_modules[] = {
		"mouse_x11",
		"mouse_linux_input",
		0
	};
	if (!env.mouse) {
		module = app->probe(mouse_modules);
		env.mouse = (Mouse *)module;
	}
}

static void probe_modules() {
	App *app = App::instance();
	EnvProbe *env_probe = (EnvProbe *)app->probe("envprobe");
	if (env_probe)
		env_probe->probe_env(&env);
	else
		probe_modules_fallback();
	runnable = (Runnable *)app->probe("frt_os_unix");
}

static void cleanup_modules() {
	if (env.mouse)
		env.mouse->cleanup();
	if (env.keyboard)
		env.keyboard->cleanup();
	if (env.video)
		env.video->cleanup();
	if (runnable)
		runnable->cleanup();
}

int main(int argc, char *argv[]) {
	int ret_code = 255;
	bool started = false;
	App::instance()->parse_args(&argc, &argv);
	probe_modules();
	if (!runnable)
		goto quit_with_ret_code;
	runnable->setup_env(&env);
	if (Main::setup(argv[0], argc - 1, &argv[1]) != OK)
		goto quit_with_ret_code;
	if (!Main::start())
		goto quit;
	started = true;
	runnable->run_();
quit:
	if (started)
		Main::cleanup();
	ret_code = runnable->get_exit_code_();
quit_with_ret_code:
	cleanup_modules();
	return ret_code;
}
