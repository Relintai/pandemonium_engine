#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include "core/string/ustring.h"

#include "core/object/reference.h"

class QueryResult;

class UnitTest : public Reference {
	GDCLASS(UnitTest, Reference);

public:
	// ThreadPoolJob like api
	// asserts, prints, file check helpers, etc etc
	// bool process() -> return true when finished
	// bool _process()

	//sub_section_start(name)
	//sub_section_end()

	// test(name); 
	// testing(name); //maybe?

	// assert(a == "b", "Testing whenther a == b")
	// assert_equals(a, "b")    ?  -> could generate text automatically

	//---
	/*
	
	bool _process() {
		testing("String int conversion.");
		assert(String::num(1) == "1");


		//or
		testing("String int conversion.");
		//cpp macro
		UTASSERT(String::num(1) == "1"); -> assert(String::num(1) == "1", __LINE__);

		sub_section_start(name)

	}

	*/
	///---

	// process type: process, physics process

	//api to get results

	//ajutomatically count tests, and some data (process time, physics and normal iteration nums)

	//get_runner() -> returns runner



	UnitTest();
	virtual ~UnitTest();

protected:
	static void _bind_methods();
};

#endif
