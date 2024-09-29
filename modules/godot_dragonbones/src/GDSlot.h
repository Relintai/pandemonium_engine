#ifndef GDSLOT_H
#define GDSLOT_H

#include <memory>
#include "dragonBones/DragonBonesHeaders.h"
#include "GDDisplay.h"

#if (VERSION_MAJOR >= 3)
	#define CLASS_BIND_GODO ClassDB
	#define METH D_METHOD
	#define _SCS(val) val

#else

	#include "core/globals.h"

	#define CLASS_BIND_GODO ObjectTypeDB
	#define METH _MD

#endif

DRAGONBONES_NAMESPACE_BEGIN

class Slot_GD : public Slot {
	BIND_CLASS_TYPE_A(Slot_GD);

private:
    float       _textureScale;
    GDDisplay*  _renderDisplay;

public:
	virtual void _updateVisible() override;
	virtual void _updateBlendMode() override;
	virtual void _updateColor() override;

protected:
    virtual void _initDisplay(void* value, bool isRetain) override;
    virtual void _disposeDisplay(void* value, bool isRelease) override;
	virtual void _onUpdateDisplay() override;
	virtual void _addDisplay() override;
	virtual void _replaceDisplay(void* value, bool isArmatureDisplay) override;
	virtual void _removeDisplay() override;
	virtual void _updateZOrder() override;

	virtual void _updateFrame() override;
	virtual void _updateMesh() override;
    virtual void _updateTransform() override;
    virtual void _identityTransform() override;

	virtual void _onClear() override;


    static void __get_uv_pt(Point2& _pt, bool _is_rot, float _u, float _v, const Rectangle& _reg, const TextureAtlasData *_p_atlas);



};

class GDSlot : public Node {
	GDCLASS(GDSlot, Node);

private:
	Slot_GD *slot;

	/* BIND METHODS */
public:

	void set_slot(Slot_GD *_slot);

	static void _bind_methods();

	Color get_display_color_multiplier();
	void set_display_color_multiplier(const Color &_color);
	void set_display_index(int index = 0);
	void set_display_by_name(const String &_name);
	int get_display_index();
	int get_display_count();
	void next_display();
	void previous_display();
	String get_slot_name();

	GDDisplay *get_child_armature();

};

DRAGONBONES_NAMESPACE_END

#endif // GDSLOT_H
