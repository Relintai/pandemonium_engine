/*
Copyright (c) 2019-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "register_types.h"

#include "database.h"
#include "database_connection.h"
#include "database_manager.h"
#include "database_multi_threaded.h"
#include "database_single_threaded.h"
#include "query_builder.h"
#include "query_result.h"
#include "table_builder.h"

#include "core/engine.h"

DatabaseManager *_database_manager = nullptr;

void register_database_types() {
	ClassDB::register_class<Database>();
	ClassDB::register_class<DatabaseConnection>();
	ClassDB::register_class<DatabaseManager>();
	ClassDB::register_class<DatabaseMultiThreaded>();
	ClassDB::register_class<DatabaseSingleThreaded>();
	ClassDB::register_class<QueryBuilder>();
	ClassDB::register_class<QueryResult>();
	ClassDB::register_class<TableBuilder>();

	_database_manager = memnew(DatabaseManager);
	Engine::get_singleton()->add_singleton(Engine::Singleton("DatabaseManager", DatabaseManager::get_singleton()));
}

void unregister_database_types() {
	if (_database_manager) {
		memdelete(_database_manager);
	}
}
