#ifndef MM_TONES_EDITOR_H
#define MM_TONES_EDITOR_H

#include "core/object/reference.h"

#include "scene/gui/box_container.h"

class MMTones;
class OptionButton;
class TextureRect;
class ImageTexture;

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
		HISTOGRAM_IMAGE_SIZE = 256,
	};

	void set_value(const Ref<MMTones> &v);

	Ref<ImageTexture> make_histogram(const Ref<Image> &img);
	Ref<ImageTexture> make_default_histogram();

	MMTonesEditor();
	~MMTonesEditor();

protected:
	void on_input_property_changed();

	static void _bind_methods();

	Ref<MMTones> _node;

	OptionButton *_mode_ob;
	TextureRect *_histogram_tr;
};

#endif
