#include<iostream>
#include "smart_ptr.hpp"
#include "testing_object.hpp"

#define POINTEUR(T) smart_ptr<T>

struct CC;
struct CC
{
    // possiblement plusieurs champs
    // dont un champ de type pointeur :
    POINTEUR(CC) p;
    CC() : p(0){}
};

int main()
{
    POINTEUR(test_obj) test  = new(0) test_obj;
    POINTEUR(test_obj) test2 = new(0) test_obj;
    POINTEUR(test_obj) test3 = new(0) test_obj;

    test->add_dep(test2);
    test2->add_dep(test3);
    
    std::cout << "-------------------------------" << std::endl;

    return 0;
}
