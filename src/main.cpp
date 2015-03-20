#include <iostream>
#include "garbage_collector.hpp"
#include "smart_ptr.hpp"
#include "testing_object.hpp"

int main() {

	smart_ptr<test_obj> test = new(1) test_obj();
	test = NULL;

	smart_ptr<int> test_int = new(0) int(2);
	test_int = NULL;
	return 0;
}
