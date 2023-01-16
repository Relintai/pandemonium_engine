#ifndef UNIT_TEST_MANAGER_H
#define UNIT_TEST_MANAGER_H

#include "core/object/object.h"

class UnitTestDB : public Object {
	GDCLASS(UnitTestDB, Object);

public:
	//UnitTests api

	//testing hint -> auto set (probably from main) if --test arg (should automatically set the scene root ast TestRunner eventually)
	//main -> --test_engine, or --load_engine_tests or --register_engine_tests ? -> register module tests
	//should probably have folders settings (also in project settings) -> only load them if necessary

	//api for registering unit tests manually -> keep them separate

	//api for getting all unit tests -> load everything from folders, append stuff manually registered -> return it

	static UnitTestDB *get_singleton();

	UnitTestDB();
	~UnitTestDB();

protected:
	static void _bind_methods();

private:
	static UnitTestDB *_instance;
};

#endif
