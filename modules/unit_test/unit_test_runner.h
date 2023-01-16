#ifndef UNIT_TEST_RUNNER_H
#define UNIT_TEST_RUNNER_H

#include "scene/main/node.h"

class UnitTestRunner : public Node {
	GDCLASS(UnitTestRunner, Node);

public:
	// after enter tree gets all tests from db, call process on them when one returns true, go to the next one, exit after done

	//should have api to easily get things that references can't get to easily but needed

	UnitTestRunner();
	~UnitTestRunner();

protected:
	static void _bind_methods();
};

#endif
