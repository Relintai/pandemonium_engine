/*************************************************************************/
/*  register_types.cpp                                                   */
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

#include "register_types.h"

#include "database.h"
#include "database_connection.h"
#include "database_manager.h"
#include "database_multi_threaded.h"
#include "database_single_threaded.h"
#include "prepared_statement.h"
#include "query_builder.h"
#include "query_result.h"
#include "table_builder.h"

#include "core/config/engine.h"

DatabaseManager *_database_manager = nullptr;

void register_database_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		_database_manager = memnew(DatabaseManager);
		ClassDB::register_class<DatabaseManager>();
		Engine::get_singleton()->add_singleton(Engine::Singleton("DatabaseManager", DatabaseManager::get_singleton()));
	}

	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<Database>();
		ClassDB::register_class<DatabaseConnection>();
		ClassDB::register_class<DatabaseMultiThreaded>();
		ClassDB::register_class<DatabaseSingleThreaded>();
		ClassDB::register_class<QueryBuilder>();
		ClassDB::register_class<QueryResult>();
		ClassDB::register_class<TableBuilder>();
		ClassDB::register_virtual_class<PreparedStatement>();
	}
}

void unregister_database_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		if (_database_manager) {
			memdelete(_database_manager);
		}
	}
}
