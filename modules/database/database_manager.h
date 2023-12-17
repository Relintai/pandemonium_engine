#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

/*************************************************************************/
/*  database_manager.h                                                   */
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

#include "core/containers/vector.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"

#include "core/object/object.h"

class Database;

class DatabaseManager : public Object {
	GDCLASS(DatabaseManager, Object);

public:
	//ddb = default database
	Ref<Database> get_ddb();
	void set_ddb(const Ref<Database> &db);

	void add_database(const Ref<Database> &db, bool set_as_default = true);
	void remove_database(const int index, const bool unset_if_default = true);
	Ref<Database> get_database(const int index);
	int get_database_count();

	Vector<Ref<Database>> get_databases();
	Vector<Variant> get_databases_bind();

	void initialized();
	void load();

	void migrate(const bool p_clear, const bool p_should_seed, const int p_seed);

	static DatabaseManager *get_singleton();

	DatabaseManager();
	~DatabaseManager();

protected:
	Vector<Ref<Database>> _databases;
	Ref<Database> _ddb;

private:
	static void _bind_methods();

	static DatabaseManager *_instance;
};

#endif
