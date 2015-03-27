#include <iostream>
#include "garbage_collector.hpp"
#include "smart_ptr.hpp"
#include "testing_object.hpp"

#define POINTEUR(T) smart_ptr<T>

int main() {
    POINTEUR(test_obj) test  = new(0) test_obj;
    POINTEUR(test_obj) test2 = new(0) test_obj;
    test->add_dep(test2);
    test2->add_dep(test);
    std::cout << "-------------------------------" << std::endl;

    return 0;
}