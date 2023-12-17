#ifndef REGISTER_MODULE_TYPES_H
#define REGISTER_MODULE_TYPES_H

/*************************************************************************/
/*  register_module_types.h                                              */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

// Note: The engine will call register_module_types in this order,
// and in reverse order (except for start and finalize) when it goes through unregister_module_types.

enum ModuleRegistrationLevel {
	// Starting initialization, on uninitialization
	MODULE_REGISTRATION_LEVEL_START = 0,

	// Set up your singletons here.
	MODULE_REGISTRATION_LEVEL_SINGLETON,

	// Set up things like resource loaders here.
	MODULE_REGISTRATION_LEVEL_CORE,

	// Set up driver level things here.
	MODULE_REGISTRATION_LEVEL_DRIVER,

	// Set up platform level things here.
	MODULE_REGISTRATION_LEVEL_PLATFORM,

	// Set up servers here
	MODULE_REGISTRATION_LEVEL_SERVER,

	// Set up scene related things here. (Mostly normal class registrations.)
	MODULE_REGISTRATION_LEVEL_SCENE,

	// Set up scene related things here. (Mostly editor class registrations.)
	MODULE_REGISTRATION_LEVEL_EDITOR,

	// Set up testing related things here. Will only get called if necessary. (Mostly test registrations.)
	MODULE_REGISTRATION_LEVEL_TEST,

	// After everything have been set up, or uninitialized.
	// Good place to change some settings, or maybe to do something like disabling an another modules's editor plugin when necessary.
	MODULE_REGISTRATION_LEVEL_FINALIZE,
};

void register_module_types(ModuleRegistrationLevel p_level);
void unregister_module_types(ModuleRegistrationLevel p_level);

#endif // REGISTER_MODULE_TYPES_H
