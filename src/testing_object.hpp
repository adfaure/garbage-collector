//
// Created by dadou on 20/03/15.
//

#ifndef _GARBAGE_COLLECTOR_TESTTING_OBJECT_H_
#define _GARBAGE_COLLECTOR_TESTTING_OBJECT_H_
//
// Created by dadou on 20/03/15.
//

#include "testing_object.hpp"
#include <iostream>

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
