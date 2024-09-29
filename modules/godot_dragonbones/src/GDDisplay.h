#ifndef 	GDISPLAY_H
#define     GDISPLAY_H

#include "scene/resources/texture.h"
#include "scene/main/node_2d.h"
#include "core/version_generated.gen.h"

DRAGONBONES_USING_NAME_SPACE;

class GDOwnerNode : public Node2D
{

#if (VERSION_MAJOR) >= 3
	#define BLENDMODE_CLASS CanvasItemMaterial
	#define VISUAL_SERVER_NAMESPACE RS
#else
	#define VISUAL_SERVER_NAMESPACE VS
	#define BLENDMODE_CLASS CanvasItem
#endif

#if (VERSION_MAJOR >= 3)
    GDCLASS(GDOwnerNode, Node2D);
#else
    OBJ_TYPE(GDOwnerNode, Node2D);
#endif
public:
    Color		    modulate;

public:
    GDOwnerNode()  { modulate = Color(1,1,1,1); }
    virtual ~GDOwnerNode() {}

    virtual void set_modulate(const Color& _col)
    {
        modulate = _col;
    }

    virtual void dispatch_event(const String& _str_type, const EventObject* _p_value) = 0;
    virtual void dispatch_snd_event(const String& _str_type, const EventObject* _p_value) = 0;
};

class GDDisplay : public GDOwnerNode
{

#if (VERSION_MAJOR >= 3)
	GDCLASS(GDDisplay, GDOwnerNode);
#else
	OBJ_TYPE(GDDisplay, GDOwnerNode);
#endif

private:
    GDDisplay(const GDDisplay&);

public:
    Ref<Texture>    texture;
#if (VERSION_MAJOR >= 3)
    CanvasItemMaterial* p_canvas_mat;
#endif

    GDOwnerNode*    p_owner;
    bool            b_debug;

public:
    GDDisplay()  {
                   p_owner = nullptr;
                   b_debug = false;
#if (VERSION_MAJOR >= 3)
                   p_canvas_mat = memnew(CanvasItemMaterial);
				   set_material(p_canvas_mat);
#endif
                 }
    virtual ~GDDisplay() {}

    virtual void set_modulate(const Color& _col)
	{
		modulate = _col;
	}


#if (VERSION_MAJOR >= 3)
	void set_blend_mode(BLENDMODE_CLASS ::BlendMode _blend)
    {
        p_canvas_mat->set_blend_mode((CanvasItemMaterial::BlendMode)_blend);
        Ref<CanvasItemMaterial> __mat = Ref<CanvasItemMaterial>(p_canvas_mat);
        set_material(__mat);
    }
#endif
};

#endif // GDISPLAY_H


