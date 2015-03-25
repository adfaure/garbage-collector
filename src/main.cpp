#include <iostream>
#include "garbage_collector.hpp"
#include "smart_ptr.hpp"
#include "testing_object.hpp"

#define POINTEUR(T) smart_ptr<T>

int main() {

    test_obj test_t;
    POINTEUR(test_obj) test = &test_t;

    std::cout << "-------------------------------" << std::endl;

        test->add_dep();
        test = NULL;

    std::cout << "-------------------------------" << std::endl;
    return 0;

}
