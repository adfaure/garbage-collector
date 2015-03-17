#include <iostream>
#include "garbage_collector.hpp"
#include "smart_ptr.hpp"

int main() {
    int *test_int, *test_int2, *test_int3, un_int;
	un_int = 2;
	test_int = new(0) int(6);
	test_int2 = new(0) int(6);
	test_int3 = new(0) int(6);

//    garbage_collector &singleton = garbage_collector::get_instance();
//    #ifdef DEBUG
//        std::cout<< "mode debug [on] \n";
//    #endif
//	//*test_int = 42;
//	smart_ptr<int> smart;
//	smart = test_int;
//	std::cout << "pointeur pointe sur " << *smart<< std::endl;
//	*smart = 4;
//	std::cout<<"pointeur pointe sur " << *smart<< std::endl;
//	smart_ptr<int> ptr2(smart);
//	smart_ptr<int> ptr3;
//	ptr3 = ptr2;
//	ptr3 = ptr3;
//
//	std::cout<<"affectation ? " << *ptr2 << std::endl;
//	ptr2 = NULL;
//	std::cout<<"pointeur pointe sur " << *smart<< std::endl;
//	if(ptr2 == NULL) {
//		std::cout << "yataaa" << std::endl;
//	} else {
//		std::cout<<"doit etre null ? " << *ptr2 << std::endl;
//	}
	return 0;
}
