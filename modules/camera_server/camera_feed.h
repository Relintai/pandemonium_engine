#ifndef CAMERA_FEED_H
#define CAMERA_FEED_H

/*************************************************************************/
/*  camera_feed.h                                                        */
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

#include "core/io/image.h"
#include "core/math/transform_2d.h"

#include "core/object/reference.h"

#include "camera_server.h"

/**
	The camera server is a singleton object that gives access to the various
	camera feeds that can be used as the background for our environment.
**/

// Lots of things need to be reworked here
// This class should only use Images, it shouldn't touch the RenderingServer at all.
// Also activate_feed() does not set _active to true, and there is no active property.
// MOst if the logic in et_ycbcr_image and others should not be needed.

class CameraFeed : public Reference {
	GDCLASS(CameraFeed, Reference);

public:
	enum FeedDataType {
		FEED_NOIMAGE, // we don't have an image yet
		FEED_RGB, // our _texture will contain a normal RGB _texture that can be used directly
		FEED_YCBCR, // our _texture will contain a YCbCr _texture that needs to be converted to RGB before output
		FEED_YCBCR_SEP, // our camera is split into two _textures, first plane contains Y data, second plane contains CbCr data
	};

	enum FeedPosition {
		FEED_UNSPECIFIED, // we have no _idea
		FEED_FRONT, // this is a camera on the front of the device
		FEED_BACK // this is a camera on the back of the device
	};

	struct FeedFormat {
		int width;
		int height;
		String format;
		int frame_numerator;
		int frame_denominator;
		uint32_t pixel_format;

		FeedFormat() {
			width = 0;
			height = 0;
			frame_numerator = 0;
			frame_denominator = 0;
			pixel_format = 0;
		}
	};

	int get_id() const;
	bool is_active() const;
	void set_active(bool p_is_active);

	String get_name() const;
	void set_name(String p_name);

	int get_base_width() const;
	int get_base_height() const;

	FeedPosition get_position() const;
	void set_position(FeedPosition p_position);

	Transform2D get_transform() const;
	void set_transform(const Transform2D &p_transform);

	Ref<Image> get_image(CameraServer::FeedImage p_which);
	RID get_texture_rid(CameraServer::FeedImage p_which);
	uint64_t get_texture_tex_id(CameraServer::FeedImage p_which);

	FeedDataType get_datatype() const;
	void set_rgb_image(const Ref<Image> &p_rgb_img);
	void set_ycbcr_image(const Ref<Image> &p_ycbcr_img);
	void set_ycbcr_images(const Ref<Image> &p_y_img, const Ref<Image> &p_cbcr_img);

	virtual bool set_format(int p_index, const Dictionary &p_parameters);
	virtual Array get_formats() const;
	virtual FeedFormat get_format() const;

	bool activate_feed();
	void deactivate_feed();

	virtual bool _activate_feed();
	virtual void _deactivate_feed();

	CameraFeed();
	CameraFeed(String p_name, FeedPosition p_position = CameraFeed::FEED_UNSPECIFIED);
	virtual ~CameraFeed();

protected:
	static void _bind_methods();

	void _free_textures();

	String _name; // _name of our camera feed
	FeedDataType _datatype; // type of _texture data stored
	FeedPosition _position; // _position of camera on the device
	Transform2D _transform; // display _transform
	int _base_width;
	int _base_height;
	Vector<FeedFormat> _formats;
	Dictionary _parameters;
	int _selected_format;

	bool _active; // only when _active do we actually update the camera _texture each frame
	RID _texture[CameraServer::FEED_IMAGES]; // _texture images needed for this
	Ref<Image> _image;

private:
	int _id; // unique _id for this, for internal use in case feeds are removed
};

VARIANT_ENUM_CAST(CameraFeed::FeedDataType);
VARIANT_ENUM_CAST(CameraFeed::FeedPosition);

#endif
