#ifndef _GARBAGE_COLLECTOR_TESTTING_OBJECT_H_
#define _GARBAGE_COLLECTOR_TESTTING_OBJECT_H_

#include "testing_object.hpp"
#include <iostream>

/** Just a testing class to ensure the behavior of our smartpointers and GC
 */
class test_obj {
    public :
        test_obj() : test(9), test_tas(new int(6)) {
            #ifdef DEBUG
                std::cout << "test_obj::~test_obj()" << std::endl;
            #endif
        }

        ~test_obj() {
            #ifdef DEBUG
                std::cout << "test_obj::~test_obj()" << std::endl;
            #endif
            free(test_tas);
        }

    private :
        int test;
        int *test_tas;
};

#endif //_GARBAGE_COLLECTOR_TESTTING_OBJECT_H_
