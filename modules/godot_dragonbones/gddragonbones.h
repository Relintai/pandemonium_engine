#ifndef GDDRAGONBONES_H
#define GDDRAGONBONES_H

#include "scene/main/node_2d.h"
#include "src/dragonBones/DragonBonesHeaders.h"
#include "src/GDFactory.h"
#include "scene/resources/texture.h"

#if (VERSION_MAJOR) >= 5
	#include "servers/rendering_server.h"
	#define TEXTURE_CLASS Texture2D
	#define REAL_VARIANT Variant::FLOAT
#else
	#include "servers/rendering_server.h"
	#define TEXTURE_CLASS Texture
	#define REAL_VARIANT Variant::REAL
#endif

DRAGONBONES_USING_NAME_SPACE;

class GDDragonBones : public GDOwnerNode
{
#if (VERSION_MAJOR >= 3)
    GDCLASS(GDDragonBones, GDOwnerNode);
#else
    OBJ_TYPE(GDDragonBones, GDOwnerNode);
#endif

public:
	// Resource class
    class GDDragonBonesResource : public Resource
    {
#if (VERSION_MAJOR >= 3)
    GDCLASS(GDDragonBonesResource, Resource);
#else
    OBJ_TYPE(GDDragonBonesResource, Resource);
#endif
	
	public:		
		GDDragonBonesResource();
        ~GDDragonBonesResource();

        void set_def_texture_path(const String& _path);
        bool load_texture_atlas_data(const String& _path);
        bool load_bones_data(const String& _path);

        String           str_default_tex_path;
        char*            p_data_texture_atlas;
        char*            p_data_bones;
	};

private:
    GDFactory*                  p_factory;
	Ref<TEXTURE_CLASS> m_texture_atlas;
    Ref<GDDragonBonesResource>  m_res;
    String                      str_curr_anim;
    GDArmatureDisplay*          p_armature;
	GDArmatureDisplay::AnimMode m_anim_mode;
    float                       f_speed;
    float                       f_progress;
    int                         c_loop;
    bool                        b_processing;
    bool                        b_active;
    bool                        b_playing;
    bool                        b_debug;
    bool                        b_inited;
    bool                        b_try_playing;
    bool                        b_flip_x;
    bool                        b_flip_y;
    bool                        b_inherit_child_material;

protected:
    void _notification(int _what);
	static void _bind_methods();

    bool _set(const StringName& _str_name, const Variant& _c_r_value);
    bool _get(const StringName& _str_name, Variant &_r_ret) const;
    void _get_property_list(List<PropertyInfo> *_p_list) const;

public:
	GDDragonBones();
    ~GDDragonBones();

    void _cleanup();

    // to initial pose current animation
    void _reset();
    void _set_process(bool _b_process, bool _b_force = false);

    void dispatch_event(const String& _str_type, const EventObject* _p_value);
    void dispatch_snd_event(const String& _str_type, const EventObject* _p_value);

    // setters/getters
	void set_resource(Ref<GDDragonBonesResource> _p_data);
    Ref<GDDragonBonesResource> get_resource();

    void set_inherit_material(bool _b_enable);
    bool is_material_inherited() const;

	void set_active(bool _b_active);
	bool is_active() const;

	void set_speed(float _f_speed);
	float get_speed() const;

	void set_texture(const Ref<TEXTURE_CLASS> &_p_texture);
	Ref<Texture> get_texture() const;

	void set_animation_process_mode(GDArmatureDisplay::AnimMode _mode);
	GDArmatureDisplay::AnimMode get_animation_process_mode() const;

#if (VERSION_MAJOR >= 3)
#else
    void set_opacity(float _f_opacity);
    float get_opacity() const;

    void set_blend_mode(CanvasItem::BlendMode _blend_mode);
     CanvasItem::BlendMode get_blend_mode() const;

     void set_modulate(const Color& _p_color);
     Color get_modulate() const;
#endif

	/**
		THESE DEPRECATED FUNCTIONS WILL BE REMOVED IN VERSION 3.2.53
	*/
    /* deprecated */ void fade_in(const String &_name_anim, float _time, int _loop, int _layer, const String &_group, GDArmatureDisplay::AnimFadeOutMode _fade_out_mode);
	/* deprecated */ void fade_out(const String &_name_anim);
    /* deprecated */ void set_debug(bool _b_debug);
	/* deprecated */ bool is_debug() const;
	/* deprecated */ void flip_x(bool _b_flip);
	/* deprecated */ bool is_fliped_x() const;
	/* deprecated */ void flip_y(bool _b_flip);
	/* deprecated */ bool is_fliped_y() const;
	/* deprecated */ String get_current_animation() const;
	/* deprecated */ String get_current_animation_on_layer(int _layer) const;
	/* deprecated */ float tell();
	/* deprecated */ void seek(float _f_p);
	/* deprecated */ float get_progress() const;
	/* deprecated */ bool has_anim(const String &_str_anim);
	/* deprecated */ bool has_slot(const String &_slot_name) const; 
	/* deprecated */ Color get_slot_display_color_multiplier(const String &_slot_name);
	/* deprecated */ void set_slot_display_color_multiplier(const String &_slot_name, const Color &_color);
	/* deprecated */ void set_slot_display_index(const String &_slot_name, int _index = 0);
	/* deprecated */ void set_slot_by_item_name(const String &_slot_name, const String &_item_name);
	/* deprecated */ void set_all_slots_by_item_name(const String &_item_name);
	/* deprecated */ int get_slot_display_index(const String &_slot_name);
	/* deprecated */ int get_total_items_in_slot(const String &_slot_name);
	/* deprecated */ void cycle_next_item_in_slot(const String &_slot_name);
	/* deprecated */ void cycle_previous_item_in_slot(const String &_slot_name);
	/* deprecated */ bool is_playing() const;
	/* deprecated */ void play(bool _b_play = true);
	/* deprecated */ void play_from_time(float _f_time);
	/* deprecated */ void play_from_progress(float _f_progress);
	/* deprecated */ void play_new_animation(const String &_str_anim, int _num_times);
	/* deprecated */ void play_new_animation_from_progress(const String &_str_anim, int _num_times, float _f_progress);
	/* deprecated */ void play_new_animation_from_time(const String &_str_anim, int _num_times, float _f_time);
	/* deprecated */ void stop(bool _b_all = false);
	/* deprecated */ inline void stop_all() { stop(true); }

	GDArmatureDisplay *get_armature();

private:
	const DragonBonesData *get_dragonbones_data() const;
	ArmatureData *get_armature_data(const String &_armature_name);
};

#endif // GDDRAGONBONES_H


