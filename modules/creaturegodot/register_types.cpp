/* register_types.cpp */

#include "register_types.h"
#include "creaturegodot.h"

void register_creaturegodot_types(ModuleRegistrationLevel p_level) {

        ClassDB::register_class<CreatureGodot>();
}

void unregister_creaturegodot_types(ModuleRegistrationLevel p_level) {
   //nothing to do here
}