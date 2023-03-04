#ifndef MM_TONES_EDITOR_H
#define MM_TONES_EDITOR_H

#include "core/object/reference.h"

#include "scene/gui/box_container.h"

class MMTones;
class OptionButton;
class TextureRect;
class ImageTexture;
class MMTonesEditorCursor;
class MMMaterial;

class MMTonesEditor : public VBoxContainer {
	GDCLASS(MMTonesEditor, VBoxContainer);

public:
	enum Modes {
		MODE_LUMINANCE = 0,
		MODE_RED,
		MODE_GREEN,
		MODE_BLUE,
		MODE_ALPHA,
	};

	enum {
		HISTOGRAM_IMAGE_SIZE = 128,
	};

	void set_value(const Ref<MMMaterial> &material, const Ref<MMTones> &v);

	enum ParameterTypes {
		PARAMETER_TYPE_IN_MIN = 0,
		PARAMETER_TYPE_IN_MID,
		PARAMETER_TYPE_IN_MAX,
		PARAMETER_TYPE_OUT_MIN,
		PARAMETER_TYPE_OUT_MAX,
	};

	void set_mode(Modes mode);

	Color get_parameter(ParameterTypes type);
	float get_parameter_current_mode(ParameterTypes type);

	void set_parameter(ParameterTypes type, const Color &val);
	void set_parameter_current_mode(ParameterTypes type, float val, float d);

	Ref<ImageTexture> make_histogram(const Ref<Image> &img);
	Ref<ImageTexture> make_default_histogram();

	MMTonesEditor();
	~MMTonesEditor();

protected:
	void on_input_property_changed();
	void on_auto_levels_pressed();
	void on_mode_item_selected(int id);
	void on_cursor_value_changed(Control *cursor, float position);

	void _notification(int p_what);

	static void _bind_methods();

	Ref<MMMaterial> _material;
	Ref<MMTones> _node;

	Modes _current_mode;

	OptionButton *_mode_ob;
	TextureRect *_histogram_tr;

	MMTonesEditorCursor *_cursor_in_min;
	MMTonesEditorCursor *_cursor_in_mid;
	MMTonesEditorCursor *_cursor_in_max;
	MMTonesEditorCursor *_cursor_out_min;
	MMTonesEditorCursor *_cursor_out_max;
};

#endif
