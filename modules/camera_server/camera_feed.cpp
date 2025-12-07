/*************************************************************************/
/*  camera_feed.cpp                                                      */
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
/*  camera_feed.cpp                                                       */

#include "camera_feed.h"

#include "servers/rendering_server.h"

int CameraFeed::get_id() const {
	return _id;
}

bool CameraFeed::is_active() const {
	return _active;
}

void CameraFeed::set_active(bool p_is_active) {
	if (p_is_active == _active) {
		// all good
	} else if (p_is_active) {
		// attempt to activate this feed
		if (activate_feed()) {
			_active = true;
		}
	} else {
		// just deactivate it
		deactivate_feed();
		_active = false;
	}
}

String CameraFeed::get_name() const {
	return _name;
}

void CameraFeed::set_name(String p_name) {
	_name = p_name;
}

int CameraFeed::get_base_width() const {
	return _base_width;
}

int CameraFeed::get_base_height() const {
	return _base_height;
}

CameraFeed::FeedDataType CameraFeed::get_datatype() const {
	return _datatype;
}

CameraFeed::FeedPosition CameraFeed::get_position() const {
	return _position;
}

void CameraFeed::set_position(CameraFeed::FeedPosition p_position) {
	_position = p_position;
}

Transform2D CameraFeed::get_transform() const {
	return _transform;
}

void CameraFeed::set_transform(const Transform2D &p_transform) {
	_transform = p_transform;
}

RID CameraFeed::get_texture(CameraServer::FeedImage p_which) {
	return _texture[p_which];
}

uint64_t CameraFeed::get_texture_tex_id(CameraServer::FeedImage p_which) {
	return RenderingServer::get_singleton()->texture_get_native_handle(_texture[p_which]);
}

void CameraFeed::set_rgb_image(const Ref<Image> &p_rgb_img) {
	ERR_FAIL_COND(p_rgb_img.is_null());
	if (_active) {
		int new_width = p_rgb_img->get_width();
		int new_height = p_rgb_img->get_height();

		// Emit `format_changed` signal if feed _datatype or frame size is changed.
		// The signal is deferred to ensure:
		// - They are emitted on Godot's main thread.
		// - Both _datatype and frame size are updated before the emission.
		if (_datatype != CameraFeed::FEED_RGB || (_base_width != new_width) || (_base_height != new_height)) {
			call_deferred("emit_signal", "format_changed");
		}

		if ((_base_width != new_width) || (_base_height != new_height)) {
			// We're assuming here that our camera image doesn't change around _formats etc, allocate the whole lot...
			_base_width = new_width;
			_base_height = new_height;

			RID new_texture = RenderingServer::get_singleton()->texture_2d_create(p_rgb_img);
			RenderingServer::get_singleton()->texture_replace(_texture[CameraServer::FEED_RGBA_IMAGE], new_texture);
		} else {
			RenderingServer::get_singleton()->texture_2d_update(_texture[CameraServer::FEED_RGBA_IMAGE], p_rgb_img);
		}

		_datatype = CameraFeed::FEED_RGB;
		// Most of the time the pixel data of camera devices comes from threads outs_ide Godot.
		// Defer `frame_changed` signals to ensure they are emitted on Godot's main thread.
		call_deferred("emit_signal", "frame_changed");
	}
}

void CameraFeed::set_ycbcr_image(const Ref<Image> &p_ycbcr_img) {
	ERR_FAIL_COND(p_ycbcr_img.is_null());
	if (_active) {
		int new_width = p_ycbcr_img->get_width();
		int new_height = p_ycbcr_img->get_height();

		// Emit `format_changed` signal if feed _datatype or frame size is changed.
		// The signal is deferred to ensure:
		// - They are emitted on Godot's main thread.
		// - Both _datatype and frame size are updated before the emission.
		if (_datatype != CameraFeed::FEED_YCBCR || (_base_width != new_width) || (_base_height != new_height)) {
			call_deferred("emit_signal", "format_changed");
		}

		if ((_base_width != new_width) || (_base_height != new_height)) {
			// We're assuming here that our camera image doesn't change around _formats etc, allocate the whole lot...
			_base_width = new_width;
			_base_height = new_height;

			RID new_texture = RenderingServer::get_singleton()->texture_2d_create(p_ycbcr_img);
			RenderingServer::get_singleton()->texture_replace(_texture[CameraServer::FEED_RGBA_IMAGE], new_texture);
		} else {
			RenderingServer::get_singleton()->texture_2d_update(_texture[CameraServer::FEED_RGBA_IMAGE], p_ycbcr_img);
		}

		_datatype = CameraFeed::FEED_YCBCR;
		// Most of the time the pixel data of camera devices comes from threads outs_ide Godot.
		// Defer `frame_changed` signals to ensure they are emitted on Godot's main thread.
		call_deferred("emit_signal", "frame_changed");
	}
}

void CameraFeed::set_ycbcr_images(const Ref<Image> &p_y_img, const Ref<Image> &p_cbcr_img) {
	ERR_FAIL_COND(p_y_img.is_null());
	ERR_FAIL_COND(p_cbcr_img.is_null());
	if (_active) {
		///@TODO investigate whether we can use thirdparty/misc/yuv2rgb.h here to convert our YUV data to RGB, our shader approach is potentially faster though..
		// Wondering about including that into multiple projects, may cause issues.
		// That sa_id, if we convert to RGB, we could enable using _texture resources again...

		int new_y_width = p_y_img->get_width();
		int new_y_height = p_y_img->get_height();

		// Emit `format_changed` signal if feed _datatype or frame size is changed.
		// The signal is deferred to ensure:
		// - They are emitted on Godot's main thread.
		// - Both _datatype and frame size are updated before the emission.
		if (_datatype != CameraFeed::FEED_YCBCR_SEP || (_base_width != new_y_width) || (_base_height != new_y_height)) {
			call_deferred("emit_signal", "format_changed");
		}

		if ((_base_width != new_y_width) || (_base_height != new_y_height)) {
			// We're assuming here that our camera image doesn't change around _formats etc, allocate the whole lot...
			_base_width = new_y_width;
			_base_height = new_y_height;
			{
				RID new_texture = RenderingServer::get_singleton()->texture_2d_create(p_y_img);
				RenderingServer::get_singleton()->texture_replace(_texture[CameraServer::FEED_Y_IMAGE], new_texture);
			}
			{
				RID new_texture = RenderingServer::get_singleton()->texture_2d_create(p_cbcr_img);
				RenderingServer::get_singleton()->texture_replace(_texture[CameraServer::FEED_CBCR_IMAGE], new_texture);
			}
		} else {
			RenderingServer::get_singleton()->texture_2d_update(_texture[CameraServer::FEED_Y_IMAGE], p_y_img);
			RenderingServer::get_singleton()->texture_2d_update(_texture[CameraServer::FEED_CBCR_IMAGE], p_cbcr_img);
		}

		_datatype = CameraFeed::FEED_YCBCR_SEP;
		// Most of the time the pixel data of camera devices comes from threads outs_ide Godot.
		// Defer `frame_changed` signals to ensure they are emitted on Godot's main thread.
		call_deferred("emit_signal", "frame_changed");
	}
}

void CameraFeed::set_external(int p_width, int p_height) {
	// Emit `format_changed` signal if feed _datatype or frame size is changed.
	// The signal is deferred to ensure:
	// - They are emitted on Godot's main thread.
	// - Both _datatype and frame size are updated before the emission.
	if (_datatype != CameraFeed::FEED_EXTERNAL || (_base_width != p_width) || (_base_height != p_height)) {
		call_deferred("emit_signal", "format_changed");
	}

	if ((_base_width != p_width) || (_base_height != p_height)) {
		// We're assuming here that our camera image doesn't change around _formats etc, allocate the whole lot...
		_base_width = p_width;
		_base_height = p_height;

		RID new_texture = RenderingServer::get_singleton()->texture_external_create(p_width, p_height, 0);
		RenderingServer::get_singleton()->texture_replace(_texture[CameraServer::FEED_YCBCR_IMAGE], new_texture);
	}

	_datatype = CameraFeed::FEED_EXTERNAL;
	// Most of the time the pixel data of camera devices comes from threads outs_ide Godot.
	// Defer `frame_changed` signals to ensure they are emitted on Godot's main thread.
	call_deferred("emit_signal", "frame_changed");
}

bool CameraFeed::activate_feed() {
	return call("_activate_feed");
}

void CameraFeed::deactivate_feed() {
	call("_deactivate_feed");
}

bool CameraFeed::_activate_feed() {
}
void CameraFeed::_deactivate_feed() {
}

bool CameraFeed::set_format(int p_index, const Dictionary &p_parameters) {
	return false;
}

Array CameraFeed::get_formats() const {
	return Array();
}

CameraFeed::FeedFormat CameraFeed::get_format() const {
	FeedFormat feed_format = {};
	return feed_format;
}

CameraFeed::CameraFeed() {
	// initialize our feed
	_id = CameraServer::get_singleton()->get_free_id();
	_base_width = 0;
	_base_height = 0;
	_selected_format = -1;
	_name = "???";
	_active = false;
	_datatype = CameraFeed::FEED_RGB;
	_position = CameraFeed::FEED_UNSPECIFIED;
	_transform = Transform2D(1.0, 0.0, 0.0, -1.0, 0.0, 1.0);
	_texture[CameraServer::FEED_Y_IMAGE] = RenderingServer::get_singleton()->texture_2d_placeholder_create();
	_texture[CameraServer::FEED_CBCR_IMAGE] = RenderingServer::get_singleton()->texture_2d_placeholder_create();
}

CameraFeed::CameraFeed(String p_name, FeedPosition p_position) {
	// initialize our feed
	_id = CameraServer::get_singleton()->get_free_id();
	_base_width = 0;
	_base_height = 0;
	_selected_format = -1;
	_name = p_name;
	_active = false;
	_datatype = CameraFeed::FEED_NOIMAGE;
	_position = p_position;
	_transform = Transform2D(1.0, 0.0, 0.0, -1.0, 0.0, 1.0);
	_texture[CameraServer::FEED_Y_IMAGE] = RenderingServer::get_singleton()->texture_2d_placeholder_create();
	_texture[CameraServer::FEED_CBCR_IMAGE] = RenderingServer::get_singleton()->texture_2d_placeholder_create();
}

CameraFeed::~CameraFeed() {
	// Free our _textures
	ERR_FAIL_NULL(RenderingServer::get_singleton());
	RenderingServer::get_singleton()->free_r_id(_texture[CameraServer::FEED_Y_IMAGE]);
	RenderingServer::get_singleton()->free_r_id(_texture[CameraServer::FEED_CBCR_IMAGE]);
}

void CameraFeed::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_id"), &CameraFeed::get_id);

	ClassDB::bind_method(D_METHOD("is_active"), &CameraFeed::is_active);
	ClassDB::bind_method(D_METHOD("set_active", "active"), &CameraFeed::set_active);

	ClassDB::bind_method(D_METHOD("get_name"), &CameraFeed::get_name);
	ClassDB::bind_method(D_METHOD("set_name", "name"), &CameraFeed::set_name);

	ClassDB::bind_method(D_METHOD("get_position"), &CameraFeed::get_position);
	ClassDB::bind_method(D_METHOD("set_position", "position"), &CameraFeed::set_position);

	// Note, for _transform some feeds may overr_ide what the user sets (such as ARKit)
	ClassDB::bind_method(D_METHOD("get_transform"), &CameraFeed::get_transform);
	ClassDB::bind_method(D_METHOD("set_transform", "transform"), &CameraFeed::set_transform);

	ClassDB::bind_method(D_METHOD("set_rgb_image", "rgb_image"), &CameraFeed::set_rgb_image);
	ClassDB::bind_method(D_METHOD("set_ycbcr_image", "ycbcr_image"), &CameraFeed::set_ycbcr_image);
	ClassDB::bind_method(D_METHOD("set_ycbcr_images", "y_image", "cbcr_image"), &CameraFeed::set_ycbcr_images);
	ClassDB::bind_method(D_METHOD("set_external", "width", "height"), &CameraFeed::set_external);
	ClassDB::bind_method(D_METHOD("get_texture_tex_id", "feed_image_type"), &CameraFeed::get_texture_tex_id);

	ClassDB::bind_method(D_METHOD("get_datatype"), &CameraFeed::get_datatype);

	ClassDB::bind_method(D_METHOD("get_formats"), &CameraFeed::get_formats);
	ClassDB::bind_method(D_METHOD("set_format", "index", "parameters"), &CameraFeed::set_format);

	BIND_VMETHOD(MethodInfo(Variant::BOOL, "_activate_feed"));
	BIND_VMETHOD(MethodInfo("_deactivate_feed"));

	ClassDB::bind_method(D_METHOD("activate_feed"), &CameraFeed::activate_feed);
	ClassDB::bind_method(D_METHOD("deactivate_feed"), &CameraFeed::deactivate_feed);
	ClassDB::bind_method(D_METHOD("_activate_feed"), &CameraFeed::_activate_feed);
	ClassDB::bind_method(D_METHOD("_deactivate_feed"), &CameraFeed::_deactivate_feed);

	ADD_SIGNAL(MethodInfo("frame_changed"));
	ADD_SIGNAL(MethodInfo("format_changed"));

	ADD_GROUP("Feed", "feed_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "feed_is_active"), "set_active", "is_active");
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "feed_transform"), "set_transform", "get_transform");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "formats"), "", "get_formats");

	BIND_ENUM_CONSTANT(FEED_NOIMAGE);
	BIND_ENUM_CONSTANT(FEED_RGB);
	BIND_ENUM_CONSTANT(FEED_YCBCR);
	BIND_ENUM_CONSTANT(FEED_YCBCR_SEP);
	BIND_ENUM_CONSTANT(FEED_EXTERNAL);

	BIND_ENUM_CONSTANT(FEED_UNSPECIFIED);
	BIND_ENUM_CONSTANT(FEED_FRONT);
	BIND_ENUM_CONSTANT(FEED_BACK);
}
