#ifndef CAMERA_SERVER_H
#define CAMERA_SERVER_H

/*************************************************************************/
/*  camera_server.h                                                      */
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

#include "core/containers/rid.h"
#include "core/object/reference.h"
#include "core/os/thread_safe.h"
#include "core/io/image.h"

/**
	The camera server is a singleton object that gives access to the various
	camera feeds that can be used as the background for our environment.
**/

class CameraFeed;

class CameraServer : public Object {
	GDCLASS(CameraServer, Object);

	_THREAD_SAFE_CLASS_

public:
	enum FeedImage {
		FEED_RGBA_IMAGE = 0,
		FEED_YCBCR_IMAGE = 0,
		FEED_Y_IMAGE = 0,
		FEED_CBCR_IMAGE = 1,
		FEED_IMAGES = 2
	};

	typedef CameraServer *(*CreateFunc)();

	static CameraServer *get_singleton();

	template <typename T>
	static void make_default() {
		_create_func = _create_builtin<T>;
	}

	static CameraServer *create() {
		CameraServer *server = _create_func ? _create_func() : memnew(CameraServer);
		return server;
	}

	static void deallocate() {
		if (_singleton) {
			memdelete(_singleton);
			_singleton = NULL;
		}
	}

	static bool has_default() {
		return _create_func;
	}

	virtual void set_monitoring_feeds(bool p_monitoring_feeds);
	_FORCE_INLINE_ bool is_monitoring_feeds() const { return _monitoring_feeds; }

	// Right now we identify our feed by it's ID when it's used in the background.
	// May see if we can change this to purely relying on CameraFeed objects or by name.
	int get_free_id();
	int get_feed_index(int p_id);
	Ref<CameraFeed> get_feed_by_id(int p_id);

	// Add and remove feeds.
	void add_feed(const Ref<CameraFeed> &p_feed);
	void remove_feed(const Ref<CameraFeed> &p_feed);

	// Get our feeds.
	Ref<CameraFeed> get_feed(int p_index);
	int get_feed_count();
	Vector<Ref<CameraFeed>> get_feeds();
	Array get_feeds_bind();

	Ref<Image> get_feed_image(int p_id, FeedImage p_image);

	// Platform lifecycle callbacks (virtual, default empty implementation).
	virtual void handle_application_pause() {}
	virtual void handle_application_resume() {}
	virtual void handle_display_rotation_change(int p_orientation) { (void)p_orientation; }

	CameraServer();
	~CameraServer();

protected:
	static void _bind_methods();

	template <typename T>
	static CameraServer *_create_builtin() {
		return memnew(T);
	}

	static CreateFunc _create_func;

	bool _monitoring_feeds;
	Vector<Ref<CameraFeed>> _feeds;

	static CameraServer *_singleton;
};

VARIANT_ENUM_CAST(CameraServer::FeedImage);

#endif
