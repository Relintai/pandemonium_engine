/*************************************************************************/
/*  rendering_server_raster.cpp                                             */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "rendering_server_raster.h"

#include "core/config/project_settings.h"
#include "core/containers/sort_array.h"
#include "core/io/marshalls.h"
#include "core/os/os.h"
#include "rendering_server_canvas.h"
#include "rendering_server_globals.h"
#include "rendering_server_scene.h"

// careful, these may run in different threads than the visual server

int RenderingServerRaster::changes[2] = { 0 };

/* BLACK BARS */

void RenderingServerRaster::black_bars_set_margins(int p_left, int p_top, int p_right, int p_bottom) {
	black_margin[MARGIN_LEFT] = p_left;
	black_margin[MARGIN_TOP] = p_top;
	black_margin[MARGIN_RIGHT] = p_right;
	black_margin[MARGIN_BOTTOM] = p_bottom;
}

void RenderingServerRaster::black_bars_set_images(RID p_left, RID p_top, RID p_right, RID p_bottom) {
	black_image[MARGIN_LEFT] = p_left;
	black_image[MARGIN_TOP] = p_top;
	black_image[MARGIN_RIGHT] = p_right;
	black_image[MARGIN_BOTTOM] = p_bottom;
}

void RenderingServerRaster::_draw_margins() {
	RSG::canvas_render->draw_window_margins(black_margin, black_image);
};

/* FREE */

void RenderingServerRaster::free(RID p_rid) {
	if (!p_rid.is_valid()) {
		ERR_FAIL_MSG("Invalid RID.");
		return;
	}

	if (RSG::storage->free(p_rid)) {
		return;
	}
	if (RSG::canvas->free(p_rid)) {
		return;
	}
	if (RSG::viewport->free(p_rid)) {
		return;
	}
	if (RSG::scene->free(p_rid)) {
		return;
	}
	if (RSG::scene_render->free(p_rid)) {
		return;
	}

	ERR_FAIL_MSG("Invalid RID.");
}

/* EVENT QUEUING */

void RenderingServerRaster::request_frame_drawn_callback(Object *p_where, const StringName &p_method, const Variant &p_userdata) {
	ERR_FAIL_NULL(p_where);
	FrameDrawnCallbacks fdc;
	fdc.object = p_where->get_instance_id();
	fdc.method = p_method;
	fdc.param = p_userdata;

	frame_drawn_callbacks.push_back(fdc);
}

void RenderingServerRaster::scenario_tick(RID p_scenario) {
	RSG::scene->_scenario_tick(p_scenario);
}

void RenderingServerRaster::scenario_pre_draw(RID p_scenario, bool p_will_draw) {
	RSG::scene->_scenario_pre_draw(p_scenario, p_will_draw);
}

void RenderingServerRaster::draw(bool p_swap_buffers, double frame_step) {
	//needs to be done before changes is reset to 0, to not force the editor to redraw
	RS::get_singleton()->emit_signal("frame_pre_draw");

	changes[0] = 0;
	changes[1] = 0;

	RSG::rasterizer->begin_frame(frame_step);

	RSG::scene->update_dirty_instances(); //update scene stuff

	RSG::viewport->draw_viewports();
	_draw_margins();
	RSG::rasterizer->end_frame(p_swap_buffers);

	while (frame_drawn_callbacks.front()) {
		Object *obj = ObjectDB::get_instance(frame_drawn_callbacks.front()->get().object);
		if (obj) {
			Variant::CallError ce;
			const Variant *v = &frame_drawn_callbacks.front()->get().param;
			obj->call(frame_drawn_callbacks.front()->get().method, &v, 1, ce);
			if (ce.error != Variant::CallError::CALL_OK) {
				String err = Variant::get_call_error_text(obj, frame_drawn_callbacks.front()->get().method, &v, 1, ce);
				ERR_PRINT("Error calling frame drawn function: " + err);
			}
		}

		frame_drawn_callbacks.pop_front();
	}
	RS::get_singleton()->emit_signal("frame_post_draw");
}
void RenderingServerRaster::sync() {
}

bool RenderingServerRaster::has_changed(ChangedPriority p_priority) const {
	switch (p_priority) {
		default: {
			return (changes[0] > 0) || (changes[1] > 0);
		} break;
		case CHANGED_PRIORITY_LOW: {
			return changes[0] > 0;
		} break;
		case CHANGED_PRIORITY_HIGH: {
			return changes[1] > 0;
		} break;
	}
}
void RenderingServerRaster::init() {
	RSG::rasterizer->initialize();
}
void RenderingServerRaster::finish() {
	if (test_cube.is_valid()) {
		free(test_cube);
	}

	RSG::rasterizer->finalize();
}

/* STATUS INFORMATION */

uint64_t RenderingServerRaster::get_render_info(RenderInfo p_info) {
	return RSG::storage->get_render_info(p_info);
}

String RenderingServerRaster::get_video_adapter_name() const {
	return RSG::storage->get_video_adapter_name();
}

String RenderingServerRaster::get_video_adapter_vendor() const {
	return RSG::storage->get_video_adapter_vendor();
}

/* TESTING */

void RenderingServerRaster::set_boot_image(const Ref<Image> &p_image, const Color &p_color, bool p_scale, bool p_use_filter) {
	redraw_request();
	RSG::rasterizer->set_boot_image(p_image, p_color, p_scale, p_use_filter);
}
void RenderingServerRaster::set_default_clear_color(const Color &p_color) {
	RSG::viewport->set_default_clear_color(p_color);
}

void RenderingServerRaster::set_shader_time_scale(float p_scale) {
	RSG::rasterizer->set_shader_time_scale(p_scale);
}

bool RenderingServerRaster::has_feature(Features p_feature) const {
	return false;
}

RID RenderingServerRaster::get_test_cube() {
	if (!test_cube.is_valid()) {
		test_cube = _make_test_cube();
	}
	return test_cube;
}

bool RenderingServerRaster::has_os_feature(const String &p_feature) const {
	return RSG::storage->has_os_feature(p_feature);
}

void RenderingServerRaster::set_debug_generate_wireframes(bool p_generate) {
	RSG::storage->set_debug_generate_wireframes(p_generate);
}

void RenderingServerRaster::call_set_use_vsync(bool p_enable) {
	OS::get_singleton()->_set_use_vsync(p_enable);
}

bool RenderingServerRaster::is_low_end() const {
	return RSG::rasterizer->is_low_end();
}
RenderingServerRaster::RenderingServerRaster() {
	RSG::canvas = memnew(RenderingServerCanvas);
	RSG::viewport = memnew(RenderingServerViewport);
	RSG::scene = memnew(RenderingServerScene);
	RSG::rasterizer = Rasterizer::create();
	RSG::storage = RSG::rasterizer->get_storage();
	RSG::canvas_render = RSG::rasterizer->get_canvas();
	RSG::scene_render = RSG::rasterizer->get_scene();

	for (int i = 0; i < 4; i++) {
		black_margin[i] = 0;
		black_image[i] = RID();
	}
}

RenderingServerRaster::~RenderingServerRaster() {
	memdelete(RSG::canvas);
	memdelete(RSG::viewport);
	memdelete(RSG::rasterizer);
	memdelete(RSG::scene);
}
