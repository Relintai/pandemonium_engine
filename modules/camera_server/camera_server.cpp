/*************************************************************************/
/*  camera_server.cpp                                                    */
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

#include "camera_server.h"

#include "core/variant/array.h"
#include "camera_feed.h"

#include "servers/rendering_server.h"

CameraServer *CameraServer::get_singleton() {
	return _singleton;
}

void CameraServer::set_monitoring_feeds(bool p_monitoring_feeds) {
	_monitoring_feeds = p_monitoring_feeds;
}

int CameraServer::get_free_id() {
	bool id_exists = true;
	int newid = 0;

	// find a free id
	while (id_exists) {
		newid++;
		id_exists = false;
		for (int i = 0; i < _feeds.size() && !id_exists; i++) {
			if (_feeds[i]->get_id() == newid) {
				id_exists = true;
			};
		};
	};

	return newid;
}

int CameraServer::get_feed_index(int p_id) {
	ERR_FAIL_COND_V_MSG(!_monitoring_feeds, -1, "CameraServer is not actively monitoring feeds; call set_monitoring_feeds(true) first.");

	for (int i = 0; i < _feeds.size(); i++) {
		if (_feeds[i]->get_id() == p_id) {
			return i;
		};
	};

	return -1;
}

Ref<CameraFeed> CameraServer::get_feed_by_id(int p_id) {
	ERR_FAIL_COND_V_MSG(!_monitoring_feeds, nullptr, "CameraServer is not actively monitoring feeds; call set_monitoring_feeds(true) first.");

	int index = get_feed_index(p_id);

	if (index == -1) {
		return NULL;
	} else {
		return _feeds[index];
	}
}

void CameraServer::add_feed(const Ref<CameraFeed> &p_feed) {
	ERR_FAIL_COND(p_feed.is_null());

	// add our feed
	_feeds.push_back(p_feed);

	print_verbose("CameraServer: Registered camera " + p_feed->get_name() + " with ID " + itos(p_feed->get_id()) + " and position " + itos(p_feed->get_position()) + " at index " + itos(_feeds.size() - 1));

	// let whomever is interested know
	emit_signal(SNAME("camera_feed_added"), p_feed->get_id());
}

void CameraServer::remove_feed(const Ref<CameraFeed> &p_feed) {
	for (int i = 0; i < _feeds.size(); i++) {
		if (_feeds[i] == p_feed) {
			int feed_id = p_feed->get_id();

			print_verbose("CameraServer: Removed camera " + p_feed->get_name() + " with ID " + itos(feed_id) + " and position " + itos(p_feed->get_position()));

			// remove it from our array, if this results in our feed being unreferenced it will be destroyed
			_feeds.remove(i);

			// let whomever is interested know
			emit_signal(SNAME("camera_feed_removed"), feed_id);
			return;
		};
	};
}

Ref<CameraFeed> CameraServer::get_feed(int p_index) {
	ERR_FAIL_COND_V_MSG(!_monitoring_feeds, nullptr, "CameraServer is not actively monitoring feeds; call set_monitoring_feeds(true) first.");
	ERR_FAIL_INDEX_V(p_index, _feeds.size(), nullptr);

	return _feeds[p_index];
}

int CameraServer::get_feed_count() {
	ERR_FAIL_COND_V_MSG(!_monitoring_feeds, 0, "CameraServer is not actively monitoring feeds; call set_monitoring_feeds(true) first.");
	return _feeds.size();
}

Vector<Ref<CameraFeed>> CameraServer::get_feeds() {
	return _feeds;
}
Array CameraServer::get_feeds_bind() {
	ERR_FAIL_COND_V_MSG(!_monitoring_feeds, {}, "CameraServer is not actively monitoring feeds; call set_monitoring_feeds(true) first.");
	Array ret;

	ret.resize(_feeds.size());

	for (int i = 0; i < _feeds.size(); ++i) {
		ret[i] = _feeds[i].get_ref_ptr();
	}

	return ret;
}

RID CameraServer::feed_texture(int p_id, CameraServer::FeedImage p_texture) {
	ERR_FAIL_COND_V_MSG(!_monitoring_feeds, RID(), "CameraServer is not actively monitoring feeds; call set_monitoring_feeds(true) first.");
	int index = get_feed_index(p_id);
	ERR_FAIL_COND_V(index == -1, RID());

	Ref<CameraFeed> feed = get_feed(index);

	return feed->get_texture(p_texture);
}

CameraServer::CameraServer() {
	_singleton = this;
	_monitoring_feeds = false;
}

CameraServer::~CameraServer() {
	_singleton = NULL;
}

void CameraServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_monitoring_feeds", "is_monitoring_feeds"), &CameraServer::set_monitoring_feeds);
	ClassDB::bind_method(D_METHOD("is_monitoring_feeds"), &CameraServer::is_monitoring_feeds);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "monitoring_feeds"), "set_monitoring_feeds", "is_monitoring_feeds");
	ADD_PROPERTY_DEFAULT("monitoring_feeds", false);

	ClassDB::bind_method(D_METHOD("get_feed", "index"), &CameraServer::get_feed);
	ClassDB::bind_method(D_METHOD("get_feed_count"), &CameraServer::get_feed_count);
	ClassDB::bind_method(D_METHOD("get_feeds"), &CameraServer::get_feeds_bind);

	ClassDB::bind_method(D_METHOD("add_feed", "feed"), &CameraServer::add_feed);
	ClassDB::bind_method(D_METHOD("remove_feed", "feed"), &CameraServer::remove_feed);

	ADD_SIGNAL(MethodInfo("camera_feed_added", PropertyInfo(Variant::INT, "id")));
	ADD_SIGNAL(MethodInfo("camera_feed_removed", PropertyInfo(Variant::INT, "id")));
	ADD_SIGNAL(MethodInfo("camera_feeds_updated"));

	BIND_ENUM_CONSTANT(FEED_RGBA_IMAGE);
	BIND_ENUM_CONSTANT(FEED_YCBCR_IMAGE);
	BIND_ENUM_CONSTANT(FEED_Y_IMAGE);
	BIND_ENUM_CONSTANT(FEED_CBCR_IMAGE);
}

CameraServer::CreateFunc CameraServer::_create_func = NULL;
CameraServer *CameraServer::_singleton = NULL;
