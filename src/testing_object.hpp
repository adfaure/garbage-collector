#ifndef _GARBAGE_COLLECTOR_TESTTING_OBJECT_H_
#define _GARBAGE_COLLECTOR_TESTTING_OBJECT_H_

#include "testing_object.hpp"
#include <iostream>

/** Just a testing class to ensure the behavior of our smartpointers and GC
 */
class test_obj {
    public :
        test_obj() : test(9), test_tas(new int(6)) {
            std::cout<< "hello ! " << std::endl;
        }

    private :
        int test;
        int *test_tas;
};

#endif //_GARBAGE_COLLECTOR_TESTTING_OBJECT_H_
