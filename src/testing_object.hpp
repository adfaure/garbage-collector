#ifndef _GARBAGE_COLLECTOR_TESTTING_OBJECT_H_
#define _GARBAGE_COLLECTOR_TESTTING_OBJECT_H_

#include <iostream>
#include "smart_ptr.hpp"

/** \brief Just a testing class to ensure the behavior of our smartpointers and GC
 */
class test_obj 
{
    public :
        test_obj() : test(9), previous(NULL), next(NULL)
        {
            #ifdef DEBUG
                std::cerr << "test_obj::test_obj()" << std::endl;
            #endif
        }

        void add_dep() {
            #ifdef DEBUG
                std::cerr << "test_obj::test_obj()" << std::endl;
            #endif
        }
    
        void add_dep(smart_ptr<test_obj> &t) {
            #ifdef DEBUG
                std::cerr << "test_obj::test_obj()" << std::endl;
            #endif
            t->previous = (this);
            this->next = t;
        }
        
        void add_simple_dep(smart_ptr<test_obj> t) {
            #ifdef DEBUG
                std::cerr << "test_obj::test_obj()" << std::endl;
            #endif
            this->next = t;
        }
        
        ~test_obj() {
                #ifdef DEBUG
                    std::cerr << "test_obj::~test_obj()" << std::endl;
                #endif
            }

    private :
        int test;
        smart_ptr<test_obj> previous;
        smart_ptr<test_obj> next;
};

#endif //_GARBAGE_COLLECTOR_TESTTING_OBJECT_H_
