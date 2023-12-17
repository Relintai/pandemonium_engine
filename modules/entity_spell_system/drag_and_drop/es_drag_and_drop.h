#ifndef ES_DRAG_AND_DROP_H
#define ES_DRAG_AND_DROP_H


#include "core/object/reference.h"
#include "core/string/ustring.h"

#include "scene/main/node.h"

class ESDragAndDrop : public Reference {
	GDCLASS(ESDragAndDrop, Reference);

public:
	static const String BINDING_STRING_ES_DRAG_AND_DROP_TYPE;

	enum ESDragAndDropType {
		ES_DRAG_AND_DROP_TYPE_NONE = 0,
		ES_DRAG_AND_DROP_TYPE_SPELL = 1,
		ES_DRAG_AND_DROP_TYPE_ITEM = 2,
		ES_DRAG_AND_DROP_TYPE_INVENTORY_ITEM = 3,
		ES_DRAG_AND_DROP_TYPE_EQUIPPED_ITEM = 4,
	};

	Node *get_origin() const;
	void set_origin(Node *owner);

	ESDragAndDropType get_type() const;
	void set_type(const ESDragAndDropType type);

	StringName get_item_path() const;
	void set_item_path(const StringName &item_path);

	ESDragAndDrop();

protected:
	static void _bind_methods();

private:
	Node *_origin;
	ESDragAndDropType _type;
	StringName _item_path;
};

VARIANT_ENUM_CAST(ESDragAndDrop::ESDragAndDropType);

#endif
