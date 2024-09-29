#include "gddragonbones.h"
#include "core/io/resource_loader.h"

#include "core/os/file_access.h"
#include "core/os/os.h"

#include "core/object/method_bind_ext.gen.inc"


#if (VERSION_MAJOR >= 3)
    #define CLASS_BIND_GODO  ClassDB
    #define METH             D_METHOD
    #define _SCS(val)        val
	
#else

#include "core/globals.h"

    #define CLASS_BIND_GODO  ObjectTypeDB
    #define METH			_MD

#endif

GDDragonBones::GDDragonBonesResource::GDDragonBonesResource()
{
    p_data_texture_atlas = nullptr;
    p_data_bones = nullptr;
}

GDDragonBones::GDDragonBonesResource::~GDDragonBonesResource()
{
    if(p_data_texture_atlas)
    {
        memfree(p_data_texture_atlas);
        p_data_texture_atlas = nullptr;
    }

    if(p_data_bones)
    {
        memfree(p_data_bones);
        p_data_bones = nullptr;
    }
}

char*    __load_file(const String& _file_path)
{
    FileAccess* __p_f = FileAccess::open(_file_path, FileAccess::READ);
    ERR_FAIL_COND_V(!__p_f, nullptr);
    ERR_FAIL_COND_V(!__p_f->get_len(), nullptr);

   // mem
    char* __p_data = (char*)memalloc(__p_f->get_len() + 1);
    ERR_FAIL_COND_V(!__p_data, nullptr);

    __p_f->get_buffer((uint8_t *)__p_data, __p_f->get_len());
    __p_data[__p_f->get_len()] = 0x00;
  
    memdelete(__p_f);

    return __p_data;
}

void       GDDragonBones::GDDragonBonesResource::set_def_texture_path(const String& _path)
{
    str_default_tex_path = _path;
}

bool       GDDragonBones::GDDragonBonesResource::load_texture_atlas_data(const String& _path)
{
    p_data_texture_atlas = __load_file(_path);
    ERR_FAIL_COND_V(!p_data_texture_atlas, false);
    return true;
}

bool       GDDragonBones::GDDragonBonesResource::load_bones_data(const String& _path)
{
    p_data_bones = __load_file(_path);
    ERR_FAIL_COND_V(!p_data_bones, false);
    return true;
}

GDDragonBones::GDDragonBones()
{

	p_factory = nullptr;
    m_res = RES();
    str_curr_anim = "[none]";
    p_armature = nullptr;
	m_anim_mode = GDArmatureDisplay::AnimMode::ANIMATION_PROCESS_IDLE;
    f_progress = 0;
    f_speed = 1.f;
    b_processing = false;
    b_active = true;
    b_playing = false;
    b_debug = false;
    c_loop = -1;
    b_inited = false;
    b_try_playing = false;
    b_flip_x = false;
    b_flip_y = false;
    b_inherit_child_material = true;
}

GDDragonBones::~GDDragonBones()
{
    _cleanup();
}

void GDDragonBones::_cleanup()
{
    b_inited = false;

	if (p_armature != nullptr) {
		p_armature->queue_delete();
		if (p_armature->get_parent() == this) {
			remove_child(p_armature);
		}
		p_armature = nullptr;
	}

    if (p_factory != nullptr) {
		memfree(p_factory);
		p_factory = nullptr;
	}

    m_res = RES();
}

void GDDragonBones::dispatch_snd_event(const String& _str_type, const EventObject* _p_value)
{
#if (VERSION_MAJOR >= 3)
    if(Engine::get_singleton()->is_editor_hint())
#else
    if(get_tree()->is_editor_hint())
#endif
        return;

   if(_str_type == EventObject::SOUND_EVENT)
       emit_signal("dragon_anim_snd_event", String(_p_value->animationState->name.c_str()), String(_p_value->name.c_str()));
}

void GDDragonBones::dispatch_event(const String& _str_type, const EventObject* _p_value)
{
#if (VERSION_MAJOR >= 3)
    if(Engine::get_singleton()->is_editor_hint())
#else
    if(get_tree()->is_editor_hint())
#endif
        return;

    if (_str_type == EventObject::START)
		emit_signal("dragon_anim_start", String(_p_value->animationState->name.c_str()));
	else if (_str_type == EventObject::LOOP_COMPLETE)
		emit_signal("dragon_anim_loop_complete", String(_p_value->animationState->name.c_str()));
	else if (_str_type == EventObject::COMPLETE)
		emit_signal("dragon_anim_complete", String(_p_value->animationState->name.c_str()));
	else if (_str_type == EventObject::FRAME_EVENT) {
		int int_val = 0;
		int float_val = 0;
		const char *string_val = std::string("").c_str();
		UserData* data = _p_value->getData();
		Armature* armature = _p_value->getArmature();

		if (data != NULL) {
				int_val = _p_value->getData()->getInt(0);
				float_val = _p_value->getData()->getFloat(0);

				if (!data->getStrings().empty()) {
					string_val = _p_value->getData()->getString(0).c_str();
				}
		}

		Dictionary dict = Dictionary();

		dict[String("armature")] = String(armature->getName().c_str());
		dict[String("animation")] = String(_p_value->animationState->name.c_str());
		dict[String("event_name")] = String(_p_value->name.c_str());
		dict[String("int")] = int_val;
		dict[String("float")] = float_val;
		dict[String("string")] = string_val;

		emit_signal("dragon_anim_event", dict);
	}
	else if (_str_type == EventObject::FADE_IN)
		emit_signal("dragon_fade_in", String(_p_value->animationState->name.c_str()));
	else if (_str_type == EventObject::FADE_IN_COMPLETE)
		emit_signal("dragon_fade_in_complete", String(_p_value->animationState->name.c_str()));
	else if (_str_type == EventObject::FADE_OUT)
		emit_signal("dragon_fade_out", String(_p_value->animationState->name.c_str()));
	else if (_str_type == EventObject::FADE_OUT_COMPLETE)
		emit_signal("dragon_fade_out_complete", String(_p_value->animationState->name.c_str()));
	
}

void GDDragonBones::set_resource(Ref<GDDragonBonesResource> _p_data)
{
    String __old_texture_path = "";
    if(m_res.is_valid())
        __old_texture_path = m_res->str_default_tex_path;
	else if(_p_data.is_valid())
        __old_texture_path = _p_data->str_default_tex_path;

    if (m_res == _p_data)
		return;

    stop();
    _cleanup();

	p_factory = memnew(GDFactory(this));

    m_res = _p_data;
    if (m_res.is_null())
    {
        m_texture_atlas = Ref<TEXTURE_CLASS>();
        ERR_PRINT("Null resources");
        _change_notify();
		return;
    }

    ERR_FAIL_COND(!m_res->p_data_texture_atlas);
    ERR_FAIL_COND(!m_res->p_data_bones);

    TextureAtlasData* __p_tad = p_factory->loadTextureAtlasData(m_res->p_data_texture_atlas, nullptr);
    ERR_FAIL_COND(!__p_tad);
    DragonBonesData* __p_dbd = p_factory->loadDragonBonesData(m_res->p_data_bones);
    ERR_FAIL_COND(!__p_dbd);

    // build Armature display
    const std::vector<std::string>& __r_v_m_names = __p_dbd->getArmatureNames();
    ERR_FAIL_COND(!__r_v_m_names.size());

    p_armature = static_cast<GDArmatureDisplay*>(p_factory->buildArmatureDisplay(__r_v_m_names[0].c_str()));

    // add children armature
    p_armature->p_owner = this;

	// To support non-texture atlas; I'd want to look around here
    if(!m_texture_atlas.is_valid() || __old_texture_path != m_res->str_default_tex_path)
        m_texture_atlas = ResourceLoader::load(m_res->str_default_tex_path);

    // correction for old version of DB tad files (Zero width, height)
    if(m_texture_atlas.is_valid())
    {
        __p_tad->height = m_texture_atlas->get_height();
        __p_tad->width = m_texture_atlas->get_width();
    }

	// update flip
	p_armature->getArmature()->setFlipX(b_flip_x);
	p_armature->getArmature()->setFlipY(b_flip_y);

    p_armature->add_parent_class(b_debug, m_texture_atlas);
    // add main armature
    add_child(p_armature);

	p_armature->force_parent_owned();
    b_inited = true;

    // update color and opacity and blending
    p_armature->update_childs(true, true);

    // update material inheritance
    p_armature->update_material_inheritance(b_inherit_child_material);


    p_armature->getArmature()->advanceTime(0);

    _change_notify();
    update();

	if (!Engine::get_singleton()->is_editor_hint()) {
		_set_process(true);
	}
}

Ref<GDDragonBones::GDDragonBonesResource> GDDragonBones::get_resource()
{
    return m_res;
}

void GDDragonBones::set_inherit_material(bool _b_enable)
{
    b_inherit_child_material = _b_enable;
    if(p_armature)
	    p_armature->update_material_inheritance(b_inherit_child_material);
}

bool GDDragonBones::is_material_inherited() const
{
    return b_inherit_child_material;
}

#if (VERSION_MAJOR >= 3)
#else
void GDDragonBones::set_opacity(float _f_opacity)
{
    GDOwnerNode::set_opacity(_f_opacity);
    if(p_armature)
        p_armature->update_childs(true);
}

float GDDragonBones::get_opacity() const
{
#ifdef TOOLS_ENABLED
    if(p_armature)
         p_armature->update_childs(true);
#endif
    return GDOwnerNode::get_opacity();
}

void GDDragonBones::set_blend_mode(CanvasItem::BlendMode _blend_mode)
{
    GDOwnerNode::set_blend_mode(_blend_mode);
    if(p_armature)
        p_armature->update_childs(false, true);
}

CanvasItem::BlendMode GDDragonBones::get_blend_mode() const
{
#ifdef TOOLS_ENABLED
    if(p_armature)
        p_armature->update_childs(false, true);
#endif
    return GDOwnerNode::get_blend_mode();
}

void GDDragonBones::set_modulate(const Color& _p_color)
{
    modulate = _p_color;
    if(p_armature)
        p_armature->update_childs(true);
}

Color GDDragonBones::get_modulate() const
{
    return modulate;
}
#endif

void GDDragonBones::fade_in(const String& _name_anim, float _time, int _loop, int _layer, const String& _group, GDArmatureDisplay::AnimFadeOutMode _fade_out_mode)
{
    // setup speed
    p_factory->set_speed(f_speed);
    if(has_anim(_name_anim))
    {
        p_armature->getAnimation()->fadeIn(_name_anim.ascii().get_data(), _time, _loop, _layer, _group.ascii().get_data(), (AnimationFadeOutMode)_fade_out_mode);
        if(!b_playing)
        {
            b_playing = true;
                _set_process(true);
        }
    }
}

void GDDragonBones::fade_out(const String& _name_anim)
{
    if(!b_inited) return;

    if(!p_armature->getAnimation()->isPlaying() 
	|| !p_armature->getAnimation()->hasAnimation(_name_anim.ascii().get_data()))
	return;

    p_armature->getAnimation()->stop(_name_anim.ascii().get_data());

    if(p_armature->getAnimation()->isPlaying())
        return;

    _set_process(false);
    b_playing = false;

    _reset();
}

void GDDragonBones::set_active(bool _b_active)
{
    if (b_active == _b_active)  return;
    b_active = _b_active;
    _set_process(b_processing, true);
}

bool GDDragonBones::is_active() const
{
    return b_active;
}

void GDDragonBones::set_debug(bool _b_debug)
{
    b_debug = _b_debug;
    if(b_inited)
        p_armature->set_debug(b_debug);
}

bool GDDragonBones::is_debug() const
{
    return b_debug;
}

void GDDragonBones::flip_x(bool _b_flip)
{
    b_flip_x = _b_flip;
    if(!p_armature)
        return;
    p_armature->getArmature()->setFlipX(_b_flip);
    p_armature->getArmature()->advanceTime(0);
}

bool GDDragonBones::is_fliped_x() const
{
    return b_flip_x;
}

void GDDragonBones::flip_y(bool _b_flip)
{
    b_flip_y = _b_flip;
    if(!p_armature)
        return;
    p_armature->getArmature()->setFlipY(_b_flip);
    p_armature->getArmature()->advanceTime(0);
}

bool GDDragonBones::is_fliped_y() const
{
    return b_flip_y;
}

void GDDragonBones::set_speed(float _f_speed)
{
    f_speed = _f_speed;
    if(b_inited)
        p_factory->set_speed(_f_speed);
}

float GDDragonBones::get_speed() const
{
	return f_speed;
}

void GDDragonBones::set_animation_process_mode(GDArmatureDisplay::AnimMode _mode)
{
	if (m_anim_mode == _mode)
		return;
	bool __pr = b_processing;
	if (__pr)
		_set_process(false);
	m_anim_mode = _mode;
	if (__pr)
		_set_process(true);
}

GDArmatureDisplay::AnimMode GDDragonBones::get_animation_process_mode() const
{
	return m_anim_mode;
}

void GDDragonBones::_notification(int _what)
{
	switch (_what)
	{
	case NOTIFICATION_ENTER_TREE:
	{
		if (!b_processing)
		{
			set_process(false);
#if (VERSION_MAJOR >= 3)
			set_physics_process(false);
#else
			set_fixed_process(false);
#endif
		}
	}
	break;

	case NOTIFICATION_READY:
	{
		if (b_playing && b_inited)
			play();
	}
	break;


	case NOTIFICATION_PROCESS:
	{
		if (m_anim_mode == GDArmatureDisplay::AnimMode::ANIMATION_PROCESS_FIXED)
			break;

		if (b_processing)
			p_factory->update(get_process_delta_time());
	}
	break;

#if (VERSION_MAJOR >= 3)
	case NOTIFICATION_PHYSICS_PROCESS:
	{

		if (m_anim_mode == GDArmatureDisplay::AnimMode::ANIMATION_PROCESS_IDLE)
			break;

		if (b_processing)
			p_factory->update(get_physics_process_delta_time());
	}
	break;
#else
	case NOTIFICATION_FIXED_PROCESS:
	{

		if (m_anim_mode == ANIMATION_PROCESS_IDLE)
			break;

		if (b_processing)
			p_factory->update(get_fixed_process_delta_time());
	}
	break;
#endif
	case NOTIFICATION_EXIT_TREE:
	{

	}
	break;
	}
}

void    GDDragonBones::_reset()
{
	p_armature->getAnimation()->reset();
}

void GDDragonBones::set_slot_display_index(const String& _slot_name, int _index) {
	if (!has_slot(_slot_name)) {
		WARN_PRINT("Slot " + _slot_name + " doesn't exist");
		return;
	}

	p_armature->getSlot(_slot_name.ascii().get_data())->setDisplayIndex(_index);
}

void GDDragonBones::set_slot_by_item_name(const String &_slot_name, const String &_item_name) {
	if (!has_slot(_slot_name)) {
		WARN_PRINT("Slot " + _slot_name + " doesn't exist");
		return;
	}

	const std::vector<DisplayData *> *rawData = p_armature->getSlot(_slot_name.ascii().get_data())->getRawDisplayDatas();

	// we only want to update the slot if there's a choice
	if (rawData->size() > 1) {
		const char *desired_item = _item_name.ascii().get_data();
		std::string NONE_STRING("none");

		if (NONE_STRING.compare(desired_item) == 0) {
			p_armature->getSlot(_slot_name.ascii().get_data())->setDisplayIndex(-1);
		}

		for (int i = 0; i < rawData->size(); i++) {
			DisplayData *display_data = rawData->at(i);

			if (display_data->name.compare(desired_item) == 0) {
				p_armature->getSlot(_slot_name.ascii().get_data())->setDisplayIndex(i);
				return;
			}
		}
	} else {
		WARN_PRINT("Slot " + _slot_name + " has only 1 item; refusing to set slot");
		return;
	}

	WARN_PRINT("Slot " + _slot_name + " has no item called \"" + _item_name);
}

void GDDragonBones::set_all_slots_by_item_name(const String& _item_name) {
	std::vector<Slot*> slots = p_armature->getArmature()->getSlots();

	for (Slot* slot : slots) {
		set_slot_by_item_name(String(slot->getName().c_str()), _item_name);
	}
}

int GDDragonBones::get_slot_display_index(const String &_slot_name) {
	if (!has_slot(_slot_name)) {
		WARN_PRINT("Slot " + _slot_name + " doesn't exist");
		return -1;
	}
	return p_armature->getSlot(_slot_name.ascii().get_data())->getDisplayIndex();
}

int GDDragonBones::get_total_items_in_slot(const String& _slot_name) {
	if (!has_slot(_slot_name)) {
		WARN_PRINT("Slot " + _slot_name + " doesn't exist");
		return -1;
	}
	return p_armature->getSlot(_slot_name.ascii().get_data())->getDisplayList().size();
}

bool GDDragonBones::has_slot(const String &_slot_name) const {
	return p_armature->getSlot(_slot_name.ascii().get_data()) != nullptr;
}

void GDDragonBones::cycle_next_item_in_slot(const String &_slot_name) {
	if (!has_slot(_slot_name)) {
		WARN_PRINT("Slot " + _slot_name + " doesn't exist");
		return;
	}

	int current_slot = get_slot_display_index(_slot_name);
	current_slot++;

	set_slot_display_index(_slot_name, current_slot < get_total_items_in_slot(_slot_name) ? current_slot : -1);
}

void GDDragonBones::cycle_previous_item_in_slot(const String &_slot_name) {
	if (!has_slot(_slot_name)) {
		WARN_PRINT("Slot " + _slot_name + " doesn't exist");
		return;
	}

	int current_slot = get_slot_display_index(_slot_name);
	current_slot--;

	set_slot_display_index(_slot_name, current_slot >= -1 ? current_slot : get_total_items_in_slot(_slot_name) - 1);
}

Color GDDragonBones::get_slot_display_color_multiplier(const String &_slot_name) {
	if (!has_slot(_slot_name)) {
		WARN_PRINT("Slot " + _slot_name + " doesn't exist");
		return Color(-1,-1,-1,-1);
	}
	ColorTransform transform(p_armature->getSlot(_slot_name.ascii().get_data())->_colorTransform);

	Color return_color;
	return_color.r = transform.redMultiplier;
	return_color.g = transform.greenMultiplier;
	return_color.b = transform.blueMultiplier;
	return_color.a = transform.alphaMultiplier;
	return return_color;
}

void GDDragonBones::set_slot_display_color_multiplier(const String &_slot_name, const Color &_color) {
	if (!has_slot(_slot_name)) {
		WARN_PRINT("Slot " + _slot_name + " doesn't exist");
		return;
	}

	ColorTransform _new_color;
	_new_color.redMultiplier = _color.r;
	_new_color.greenMultiplier = _color.g;
	_new_color.blueMultiplier = _color.b;
	_new_color.alphaMultiplier = _color.a;

	p_armature->getSlot(_slot_name.ascii().get_data())->_setColor(_new_color);
}

void GDDragonBones::play(bool _b_play) {
    b_playing = _b_play;
    if(!_b_play)
    {
        stop();
        return;
    }
	_set_process(true);

    // setup speed
    p_factory->set_speed(f_speed);
    if(has_anim(str_curr_anim))
    {
        p_armature->getAnimation()->play(str_curr_anim.ascii().get_data(), c_loop);
        b_try_playing = false;
    } 

    else // not finded animation stop playing
    {
        b_try_playing = true;
        str_curr_anim  = "[none]";
        stop();
    }
}

void  GDDragonBones::play_from_time(float _f_time)
{
    play();
    if(b_playing)
         p_armature->getAnimation()->gotoAndPlayByTime(str_curr_anim.ascii().get_data(), _f_time, c_loop);
}

void   GDDragonBones::play_from_progress(float _f_progress)
{
    play();
    if(b_playing)
         p_armature->getAnimation()->gotoAndPlayByProgress(str_curr_anim.ascii().get_data(), CLAMP(_f_progress, 0, 1.f), c_loop);
}

void GDDragonBones::play_new_animation_from_progress(const String &_str_anim, int _num_times, float _f_progress) {
	stop_all();
	_set("current_animation", _str_anim);
	_set("current_loop", _num_times);
	play(true);

	play_from_progress(_f_progress);
}

void GDDragonBones::play_new_animation_from_time(const String &_str_anim, int _num_times, float _f_time) {
	stop_all();
	_set("current_animation", _str_anim);
	_set("current_loop", _num_times);
	play(true);

	play_from_time(_f_time);
}

void GDDragonBones::play_new_animation(const String &_str_anim, int _num_times) {
	stop_all();
	_set("current_animation", _str_anim);
	_set("current_loop", _num_times);
	play(true);
}

bool GDDragonBones::has_anim(const String& _str_anim)
{
	return p_armature->has_animation(_str_anim);
}

void GDDragonBones::stop(bool _b_all)
{
    if(!b_inited) return;

    _set_process(false);
    b_playing = false;

    if(p_armature->getAnimation()->isPlaying())
        p_armature->getAnimation()->stop(_b_all?"":str_curr_anim.ascii().get_data());

    _reset();
}

const DragonBonesData *GDDragonBones::get_dragonbones_data() const {
	const DragonBonesData* data = this->p_armature->getArmature()->getArmatureData()->getParent();
	return data;
}

ArmatureData* GDDragonBones::get_armature_data(const String &_armature_name) {
	std::map<std::string, ArmatureData *>::const_iterator data = get_dragonbones_data()->armatures.find(_armature_name.ascii().get_data());

	if (data == get_dragonbones_data()->armatures.end()) {
		return nullptr;
	}

	return data->second;
}

GDArmatureDisplay *GDDragonBones::get_armature() {
	return p_armature;
}

float GDDragonBones::tell()
{
    if(b_inited && has_anim(str_curr_anim))
    {
        AnimationState* __p_state = p_armature->getAnimation()->getState(str_curr_anim.ascii().get_data());
        if(__p_state)
            return __p_state->getCurrentTime()/__p_state->_duration;
    }
    return 0;
}

void GDDragonBones::seek(float _f_p)
{
    if(b_inited && has_anim(str_curr_anim))
    {
	f_progress = _f_p;
        stop();
	auto __c_p = Math::fmod(_f_p, 1.0f);
	if (__c_p == 0 && _f_p != 0)
		__c_p = 1.0f;
        p_armature->getAnimation()->gotoAndStopByProgress(str_curr_anim.ascii().get_data(), __c_p < 0?1.+__c_p:__c_p);
    }
}

float GDDragonBones::get_progress() const
{
    return f_progress;
}

bool GDDragonBones::is_playing() const
{
    return b_inited && b_playing && p_armature->getAnimation()->isPlaying();
}

String GDDragonBones::get_current_animation() const
{
    if(!b_inited || !p_armature->getAnimation())
        return String("");
    return String(p_armature->getAnimation()->getLastAnimationName().c_str());
}

String GDDragonBones::get_current_animation_on_layer(int _layer) const {

	if (!b_inited || !p_armature->getAnimation())
		return String("");
	std::vector<AnimationState *> states = p_armature->getAnimation()->getStates();

	for (AnimationState* state : states) {
		if (state->layer == _layer) {
			return state->getName().c_str();
		}
	}

	return String("");
}

void GDDragonBones::_set_process(bool _b_process, bool _b_force)
{
    if (b_processing == _b_process && !_b_force)
        return;

    switch (m_anim_mode)
    {

#if (VERSION_MAJOR >= 3)
		case GDArmatureDisplay::AnimMode::ANIMATION_PROCESS_FIXED: set_physics_process(_b_process && b_active); break;
#else
		case GDArmatureDisplay::AnimMode::ANIMATION_PROCESS_FIXED: set_fixed_process(_b_process && b_active); break;
#endif
		case GDArmatureDisplay::AnimMode::ANIMATION_PROCESS_IDLE: set_process(_b_process && b_active); break;
    }
    b_processing = _b_process;
}

void GDDragonBones::set_texture(const Ref<TEXTURE_CLASS> &_p_texture) {

    if (_p_texture.is_valid()
            && m_texture_atlas.is_valid()
            && (_p_texture == m_texture_atlas
               || m_texture_atlas->get_height() != _p_texture->get_height()
               || m_texture_atlas->get_width()  != _p_texture->get_width()))
        return;

    m_texture_atlas = _p_texture;

#ifdef DEBUG_ENABLED
    if (m_texture_atlas.is_valid()) {
#if (VERSION_MAJOR < 6)
       m_texture_atlas->set_flags(m_texture_atlas->get_flags()); //remove repeat from texture, it looks bad in sprites
#endif
//        m_texture_atlas->connect(CoreStringNames::get_singleton()->changed, this, SceneStringNames::get_singleton()->update);
    }
#endif

    if(p_armature)
    {
        p_armature->update_texture_atlas(m_texture_atlas);
        update();
    }
}

Ref<Texture> GDDragonBones::get_texture() const
{
    return m_texture_atlas;
}

bool GDDragonBones::_set(const StringName& _str_name, const Variant& _c_r_value)
{
    String name = _str_name;

    if (name == "current_animation")
    {
		if(str_curr_anim == _c_r_value)
			return false;

		str_curr_anim = _c_r_value;
		if (b_inited)
		{
			if (str_curr_anim == "[none]")
				stop();
			else if (has_anim(str_curr_anim))
			{
				if(b_playing || b_try_playing)
					play();
				else
					p_armature->getAnimation()->gotoAndStopByProgress(str_curr_anim.ascii().get_data());
			}
		}
	}
	else if (name == "current_loop")
	{
		c_loop = _c_r_value;
		if (b_inited && b_playing)
		{
			_reset();
			play();
		}
	}
	else if (name == "current_progress")
	{
		seek(_c_r_value);
	}

	return true;
}

bool GDDragonBones::_get(const StringName& _str_name, Variant &_r_ret) const
{

    String __name = _str_name;

    if (__name == "current_animation")
        _r_ret = str_curr_anim;
    else if (__name == "current_loop")
        _r_ret = c_loop;
    else if (__name == "current_progress")
        _r_ret = get_progress();
    return true;
}

void GDDragonBones::_bind_methods()
{    
#if (VERSION_MAJOR >= 3)
    ClassDB::bind_method(D_METHOD("set_texture", "texture"), &GDDragonBones::set_texture);
    ClassDB::bind_method(D_METHOD("get_texture"), &GDDragonBones::get_texture);
#else
    ClassDB::bind_method(D_METHOD("set_texture", "texture:Texture"), &GDDragonBones::set_texture);
    ClassDB::bind_method(D_METHOD("get_texture:Texture"), &GDDragonBones::get_texture);
#endif

    ClassDB::bind_method(D_METHOD("set_resource", "dragonbones"), &GDDragonBones::set_resource);
    ClassDB::bind_method(D_METHOD("get_resource"), &GDDragonBones::get_resource);

    ClassDB::bind_method(D_METHOD("set_inherit_material"), &GDDragonBones::set_inherit_material);
    ClassDB::bind_method(D_METHOD("is_material_inherited"), &GDDragonBones::is_material_inherited);




	/*
		All these functions act upon the base armature / display; a structure is being formed to make them available for all displays and armatures
	*/
#if (VERSION_MAJOR >= 3)
#else
	ClassDB::bind_method(D_METHOD("set_modulate", "modulate"), &GDDragonBones::set_modulate);
	ClassDB::bind_method(D_METHOD("get_modulate"), &GDDragonBones::get_modulate);

	ClassDB::bind_method(D_METHOD("set_opacity", "opacity"), &GDDragonBones::set_opacity);
	ClassDB::bind_method(D_METHOD("get_opacity"), &GDDragonBones::get_opacity);

	ClassDB::bind_method(D_METHOD("set_blend_mode", "blend_mode"), &GDDragonBones::set_blend_mode);
	ClassDB::bind_method(D_METHOD("get_blend_mode"), &GDDragonBones::get_blend_mode);
#endif

	ClassDB::bind_method(D_METHOD("fade_in", "anim_name", "time", "loop", "layer", "group", "fade_out_mode"), &GDDragonBones::fade_in);
	ClassDB::bind_method(D_METHOD("fade_out", "anim_name"), &GDDragonBones::fade_out);

	ClassDB::bind_method(D_METHOD("stop","_b_all"), &GDDragonBones::stop);
	ClassDB::bind_method(D_METHOD("stop_all"), &GDDragonBones::stop_all);
	ClassDB::bind_method(D_METHOD("reset"), &GDDragonBones::_reset);
	ClassDB::bind_method(D_METHOD("has_slot","_slot_name"), &GDDragonBones::has_slot);
	ClassDB::bind_method(D_METHOD("set_slot_by_item_name","_slot_name","_item_name"), &GDDragonBones::set_slot_by_item_name);
	ClassDB::bind_method(D_METHOD("set_all_slots_by_item_name","_item_name"), &GDDragonBones::set_all_slots_by_item_name);
	ClassDB::bind_method(D_METHOD("set_slot_display_index","_slot_name","_index"), &GDDragonBones::set_slot_display_index);
	ClassDB::bind_method(D_METHOD("get_slot_display_index","_slot_name"), &GDDragonBones::get_slot_display_index);
	ClassDB::bind_method(D_METHOD("get_total_items_in_slot","_slot_name"), &GDDragonBones::get_total_items_in_slot);
	ClassDB::bind_method(D_METHOD("set_slot_display_color_multiplier","_slot_name","_color"), &GDDragonBones::set_slot_display_color_multiplier);
	ClassDB::bind_method(D_METHOD("get_slot_display_color_multiplier","_slot_name"), &GDDragonBones::get_slot_display_color_multiplier);
	ClassDB::bind_method(D_METHOD("cycle_next_item_in_slot","_slot_name"), &GDDragonBones::cycle_next_item_in_slot);
	ClassDB::bind_method(D_METHOD("cycle_previous_item_in_slot","_slot_name"), &GDDragonBones::cycle_previous_item_in_slot);

    ClassDB::bind_method(D_METHOD("play","_b_play"), &GDDragonBones::play);
    ClassDB::bind_method(D_METHOD("play_from_time","_f_time"), &GDDragonBones::play_from_time);
	ClassDB::bind_method(D_METHOD("play_from_progress","_f_progress"), &GDDragonBones::play_from_progress);
	ClassDB::bind_method(D_METHOD("play_new_animation","_str_anim","_num_times"), &GDDragonBones::play_new_animation);
	ClassDB::bind_method(D_METHOD("play_new_animation_from_progress","_str_anim","_num_times","_f_progress"), &GDDragonBones::play_new_animation_from_progress);
	ClassDB::bind_method(D_METHOD("play_new_animation_from_time","_str_anim","_num_times","_f_time"), &GDDragonBones::play_new_animation_from_time);

	
    ClassDB::bind_method(D_METHOD("flip_x", "enable_flip"), &GDDragonBones::flip_x);
	ClassDB::bind_method(D_METHOD("is_fliped_x"), &GDDragonBones::is_fliped_x);
	ClassDB::bind_method(D_METHOD("flip_y", "enable_flip"), &GDDragonBones::flip_y);
	ClassDB::bind_method(D_METHOD("is_fliped_y"), &GDDragonBones::is_fliped_y);

	ClassDB::bind_method(D_METHOD("set_speed", "speed"), &GDDragonBones::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &GDDragonBones::get_speed);

	
    ClassDB::bind_method(D_METHOD("seek", "pos"), &GDDragonBones::seek);
	ClassDB::bind_method(D_METHOD("tell"), &GDDragonBones::tell);
	ClassDB::bind_method(D_METHOD("get_progress"), &GDDragonBones::get_progress);

	ClassDB::bind_method(D_METHOD("has", "name"), &GDDragonBones::has_anim);
	ClassDB::bind_method(D_METHOD("is_playing"), &GDDragonBones::is_playing);

	ClassDB::bind_method(D_METHOD("get_current_animation"), &GDDragonBones::get_current_animation);
	ClassDB::bind_method(D_METHOD("get_current_animation_on_layer","_layer"), &GDDragonBones::get_current_animation_on_layer);
	ClassDB::bind_method(D_METHOD("get_armature"), &GDDragonBones::get_armature);
	/*
		END OF BASE ARMATURE FUNCTIONS
	*/


	/*
		Let's get into the business of playing with child armatures.
	*/


    ClassDB::bind_method(D_METHOD("set_active", "active"), &GDDragonBones::set_active);
    ClassDB::bind_method(D_METHOD("is_active"), &GDDragonBones::is_active);

    ClassDB::bind_method(D_METHOD("set_debug", "debug"), &GDDragonBones::set_debug);
    ClassDB::bind_method(D_METHOD("is_debug"), &GDDragonBones::is_debug);

    ClassDB::bind_method(D_METHOD("set_animation_process_mode","mode"),&GDDragonBones::set_animation_process_mode);
    ClassDB::bind_method(D_METHOD("get_animation_process_mode"),&GDDragonBones::get_animation_process_mode);

	// This is how we set top level properties
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), _SCS("set_texture"), _SCS("get_texture"));
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "debug"), _SCS("set_debug"), _SCS("is_debug"));
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flipX"), _SCS("flip_x"), _SCS("is_fliped_x"));
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flipY"), _SCS("flip_y"), _SCS("is_fliped_y"));

#if (VERSION_MAJOR >= 3)
#else
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "modulate"), _SCS("set_modulate"), _SCS("get_modulate"));
#endif

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "resource", PROPERTY_HINT_RESOURCE_TYPE, "GDDragonBonesResource"), _SCS("set_resource"), _SCS("get_resource"));

    ADD_PROPERTY(PropertyInfo(Variant::INT, "current_mode", PROPERTY_HINT_ENUM, "Fixed,Idle"), _SCS("set_animation_process_mode"), _SCS("get_animation_process_mode"));
	ADD_PROPERTY(PropertyInfo(REAL_VARIANT, "current_speed", PROPERTY_HINT_RANGE, "-10,10,0.01"), _SCS("set_speed"), _SCS("get_speed"));
	ADD_PROPERTY(PropertyInfo(REAL_VARIANT, "current_progress", PROPERTY_HINT_RANGE, "-100,100,0.010"), _SCS("seek"), _SCS("get_progress"));
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "current_play"), _SCS("play"), _SCS("is_playing"));

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "childs use this material"), _SCS("set_inherit_material"), _SCS("is_material_inherited"));

    ADD_SIGNAL(MethodInfo("dragon_anim_start", PropertyInfo(Variant::STRING, "anim")));
    ADD_SIGNAL(MethodInfo("dragon_anim_complete", PropertyInfo(Variant::STRING, "anim")));
	ADD_SIGNAL(MethodInfo("dragon_anim_event", PropertyInfo(Variant::DICTIONARY, "event")));
    ADD_SIGNAL(MethodInfo("dragon_anim_loop_complete", PropertyInfo(Variant::STRING, "anim")));
    ADD_SIGNAL(MethodInfo("dragon_anim_snd_event", PropertyInfo(Variant::STRING, "anim"), PropertyInfo(Variant::STRING, "ev")));
    ADD_SIGNAL(MethodInfo("dragon_fade_in", PropertyInfo(Variant::STRING, "anim")));
    ADD_SIGNAL(MethodInfo("dragon_fade_in_complete", PropertyInfo(Variant::STRING, "anim")));
    ADD_SIGNAL(MethodInfo("dragon_fade_out", PropertyInfo(Variant::STRING, "anim")));
	ADD_SIGNAL(MethodInfo("dragon_fade_out_complete", PropertyInfo(Variant::STRING, "anim")));


}

void GDDragonBones::_get_property_list(List<PropertyInfo>* _p_list) const
{
    List<String> __l_names;

    if (b_inited && p_armature->getAnimation())
    {
        auto __names = p_armature->getAnimation()->getAnimationNames();
        auto __it = __names.cbegin();
        while(__it != __names.cend())
        {
            __l_names.push_back(__it->c_str());
            ++__it;
        }
    }

    __l_names.sort();
    __l_names.push_front("[none]");
    String __str_hint;
    for (List<String>::Element* __p_E = __l_names.front(); __p_E; __p_E = __p_E->next())
    {
       if (__p_E != __l_names.front())
           __str_hint += ",";
           __str_hint += __p_E->get();
    }

    _p_list->push_back(PropertyInfo(Variant::STRING, "current_animation", PROPERTY_HINT_ENUM, __str_hint));
    _p_list->push_back(PropertyInfo(Variant::INT, "current_loop", PROPERTY_HINT_RANGE, "-1,100,1"));
}
