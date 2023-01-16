
#include "unit_test_db.h"

UnitTestDB *UnitTestDB::_instance;

UnitTestDB *UnitTestDB::get_singleton() {
	return _instance;
}

UnitTestDB::UnitTestDB() {
	_instance = this;
}

UnitTestDB::~UnitTestDB() {
	_instance = NULL;
}

void UnitTestDB::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("", ""), &UnitTestDB::);
}
