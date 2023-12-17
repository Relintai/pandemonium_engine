#ifndef SKELETON_MODEL_ENTRY_H
#define SKELETON_MODEL_ENTRY_H


#include "core/object/reference.h"

#include "../data/items/model_visual_entry.h"

class SkeletonModelEntry : public Reference {
	GDCLASS(SkeletonModelEntry, Reference);

public:
	int get_count() const;
	void set_count(const int value);

	int get_priority() const;
	void set_priority(const int value);

	Color get_color() const;
	void set_color(const Color value);

	Ref<ModelVisualEntry> get_entry();
	void set_entry(Ref<ModelVisualEntry> entry);

	SkeletonModelEntry();
	~SkeletonModelEntry();

protected:
	static void _bind_methods();

private:
	int _count;
	int _priority;
	Color _color;
	Ref<ModelVisualEntry> _entry;
};

#endif
