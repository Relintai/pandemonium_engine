#ifndef INPUT_PROFILE_H
#define INPUT_PROFILE_H


#include "core/object/reference.h"
#include "core/string/ustring.h"

class ClassProfile;

class InputProfile : public Reference {
	GDCLASS(InputProfile, Reference);

public:
	Ref<ClassProfile> get_owner();
	void set_owner(ClassProfile *owner);

	Array get_data();
	void set_data(const Array &data);

	void load_to_projectsettings();
	void save_from_projectsettings();

	void load_defaults();

	void emit_change();

	Dictionary to_dict() const;
	void from_dict(const Dictionary &dict);

	InputProfile();

protected:
	static void _bind_methods();

private:
	Array _data;

	ClassProfile *_owner;
};

#endif
