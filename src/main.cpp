#include <iostream>
#include "garbage_collector.hpp"
#include "smart_ptr.hpp"
#include "testing_object.hpp"

#define POINTEUR(T) smart_ptr<T>

int main() {
    POINTEUR(test_obj) test = new(0) test_obj;
    POINTEUR(test_obj) test2 = new(0) test_obj;
    POINTEUR(test_obj) test3 = new(0) test_obj;
    POINTEUR(test_obj) test4 = new(0) test_obj;

    test->add_dep(test2);
    test2->add_dep(test3);
    test3->add_dep(test4);
    test4->add_dep(test);

    test = NULL;
    test2 = NULL;
    test3 = NULL;
    test4 = NULL;

    std::cout << "-------------------------------" << std::endl;

    std::cout << "-------------------------------" << std::endl;
    return 0;

}

