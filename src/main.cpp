#include <iostream>
#include "garbage_collector.hpp"
#include "smart_ptr.hpp"
#include <cstdlib>

int main() {

	smart_ptr<int> ptr = new(5) int(6);

	smart_ptr<int> ptr_2 = NULL;
	ptr_2 = ptr;

	ptr = new(0) int(12);
	std::cout << "ptr : " << *ptr_2 << std::endl;
	std::cout << "ptr : " << *ptr << std::endl;

	return 0;
}
