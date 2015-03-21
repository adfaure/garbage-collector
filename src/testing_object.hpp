#ifndef _GARBAGE_COLLECTOR_TESTTING_OBJECT_H_
#define _GARBAGE_COLLECTOR_TESTTING_OBJECT_H_

#include "testing_object.hpp"
#include <iostream>
#include "smart_ptr.hpp"

/** Just a testing class to ensure the behavior of our smartpointers and GC
 */
class test_obj {
    public :
        test_obj() : test(9), test_tas(NULL) {
            #ifdef DEBUG
                std::cout << "test_obj::~test_obj()" << std::endl;
            #endif
        }

        ~test_obj() {
            #ifdef DEBUG
                std::cout << "test_obj::~test_obj()" << std::endl;
            #endif
            test_tas = NULL;
        }

    private :
        int test;
        smart_ptr<int> test_tas;
};

#endif //_GARBAGE_COLLECTOR_TESTTING_OBJECT_H_
