#ifndef PROP_2D_DATA_SPRITE_H
#define PROP_2D_DATA_SPRITE_H


#include "core/math/color.h"

#include "scene/resources/texture.h"

#include "prop_2d_data_entry.h"

class Prop2DDataSprite : public Prop2DDataEntry {
	GDCLASS(Prop2DDataSprite, Prop2DDataEntry);

public:
	Ref<Texture> get_texture() const;
	void set_texture(const Ref<Texture> &value);

	Ref<Texture> get_normal_map() const;
	void set_normal_map(const Ref<Texture> &value);

	bool get_centered() const;
	void set_centered(const bool value);

	Vector2 get_offset() const;
	void set_offset(const Vector2 &value);

	bool get_flip_h() const;
	void set_flip_h(const bool value);

	bool get_flip_v() const;
	void set_flip_v(const bool value);

	int get_animation_h_frames() const;
	void set_animation_h_frames(const int value);

	int get_animation_v_frames() const;
	void set_animation_v_frames(const int value);

	int get_animation_frame() const;
	void set_animation_frame(const int value);

	Vector2 get_animation_frame_coords() const;
	void set_animation_frame_coords(const Vector2 &value);

	bool get_region_enabled() const;
	void set_region_enabled(const bool value);

	Rect2 get_region() const;
	void set_region(const Rect2 &value);

	bool get_filter_clip() const;
	void set_filter_clip(const bool value);

	bool _processor_handles(Node *node);
	void _processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform, Ref<Prop2DDataEntry> entry = Ref<Prop2DDataEntry>());
	Node *_processor_get_node_for(const Transform2D &transform, Node *node = nullptr);

	Prop2DDataSprite();
	~Prop2DDataSprite();

protected:
#ifndef DISABLE_DEPRECATED
	// TODO REMOVE AFTER NEXT RELEASE
	bool _set(const StringName &p_name, const Variant &p_value);
#endif
	static void _bind_methods();

private:
	Ref<Texture> _texture;
	Ref<Texture> _normal_map;

	bool _centered;
	Vector2 _offset;
	bool _flip_h;
	bool _flip_v;

	int _animation_h_frames;
	int _animation_v_frames;
	int _animation_frame;
	Vector2 _animation_frame_coords;

	bool _region_enabled;
	Rect2 _region;

	bool _filter_clip;
};

#endif
