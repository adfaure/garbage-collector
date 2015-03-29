#include<iostream>
#include "smart_ptr.hpp"
#include "testing_object.hpp"
#include <string>
#define POINTEUR(T) smart_ptr<T>

int main() {
    // recuperation de l'instance du garbage collector (à ne pas faire ...)
    garbage_collector &gc = garbage_collector::get_instance();
    
    std::cout<< "press any key to run the simple case "  << std::endl;
    std::cin.ignore();
    std::cout << "-------------------------------------------------------------------" <<std::endl;
    std::cout << "|                                                                 |" <<std::endl;
    std::cout << "|                        Simple case                              |" <<std::endl;
    std::cout << "|                                                                 |" <<std::endl;
    std::cout << "-------------------------------------------------------------------" <<std::endl;
    
    std::cout<< "******************************************************BEGIN******************************************************"   << std::endl;
    gc.print_state();
    
    {
        POINTEUR(test_obj) test1 = new(0) test_obj();
        std::cout<< "******************************************************BEFORE******************************************************"   << std::endl;
        gc.print_state();
    }

    gc.print_state();
    std::cout<< "******************************************************END******************************************************"   << std::endl;

    std::cout<< "press any key to run the Dependencies in heap "   << std::endl;
    std::cin.ignore();
    std::cout << std::string(50, '\n');
    std::cout << "-------------------------------------------------------------------" <<std::endl;
    std::cout << "|                                                                 |" <<std::endl;
    std::cout << "|                        Dependencies in heap                     |" <<std::endl;
    std::cout << "|                                                                 |" <<std::endl;
    std::cout << "-------------------------------------------------------------------" <<std::endl;
    
    std::cout<< "******************************************************BEGIN******************************************************"   << std::endl;
    gc.print_state();
    
    {
        POINTEUR(test_obj) test1 = new(0) test_obj();
        test1->add_dep();
        std::cout<< "******************************************************BEFORE******************************************************"   << std::endl;
        gc.print_state();
    }
    
    gc.full_garbage_collection();
    gc.small_garbage_collection();
    
    gc.print_state();
    std::cout<< "******************************************************END******************************************************"   << std::endl;
    
    std::cout<< "press any key to run the Simple cycle "  << std::endl;
    std::cin.ignore();
    std::cout << std::string(50, '\n');
    std::cout << "-------------------------------------------------------------------" <<std::endl;
    std::cout << "|                                                                 |" <<std::endl;
    std::cout << "|                        Simple cycle                             |" <<std::endl;
    std::cout << "|                                                                 |" <<std::endl;
    std::cout << "-------------------------------------------------------------------" <<std::endl;

    std::cout<< "******************************************************BEGIN******************************************************"   << std::endl;
    gc.print_state();

    {
        POINTEUR(test_obj) test1 = new(0) test_obj();
        POINTEUR(test_obj) test2 = new(0) test_obj();        
        POINTEUR(test_obj) test3 = new(0) test_obj();
        test1->add_simple_dep(test2);
        test2->add_simple_dep(test3);
        test3->add_simple_dep(test1);
        std::cout<< "******************************************************BEFORE******************************************************"   << std::endl;
        gc.print_state();

    }
    
    gc.full_garbage_collection();
    gc.small_garbage_collection();

    gc.print_state();
    std::cout<< "******************************************************END******************************************************"   << std::endl;

    
    std::cout<< "press any key to run the Strong connected "  << std::endl;
    std::cin.ignore();
    std::cout << "-------------------------------------------------------------------" <<std::endl;
    std::cout << "|                                                                 |" <<std::endl;
    std::cout << "|                        Strong connected                         |" <<std::endl;
    std::cout << "|                                                                 |" <<std::endl;
    std::cout << "-------------------------------------------------------------------" <<std::endl;
    
    
    std::cout<< "******************************************************BEGIN******************************************************"   << std::endl;
    gc.print_state();
    
    {
        POINTEUR(test_obj) testa = new(0) test_obj();
        POINTEUR(test_obj) testb = new(0) test_obj();        
        POINTEUR(test_obj) testc = new(0) test_obj();
        
        testa->add_dep(testb);
        testa->add_dep(testc);
        std::cout<< "******************************************************BEFORE******************************************************"   << std::endl;
        gc.print_state();
    }
    
    gc.full_garbage_collection();
    gc.small_garbage_collection();
    gc.print_state();
    std::cout<< "******************************************************END******************************************************"   << std::endl;
    
    std::cout<< "press any key to run the bonus "  << std::endl;
    std::cin.ignore();
    std::cout << std::string(50, '\n');
    std::cout << "-------------------------------------------------------------------" <<std::endl;
    std::cout << "|                                                                 |" <<std::endl;
    std::cout << "|                  \"efficace et robuste ?\"                      |" <<std::endl;
    std::cout << "|                                                                 |" <<std::endl;
    std::cout << "-------------------------------------------------------------------" <<std::endl;
 
     
    std::cout<< "******************************************************BEGIN******************************************************"   << std::endl;
    gc.print_state();
    
    {
        POINTEUR(test_obj) testa = new(0) test_obj();
        POINTEUR(test_obj) testb = new(0) test_obj();        
        POINTEUR(test_obj) testc = new(0) test_obj();
        int i = 0;
        while(++i < 100) {
            testa = new(0) test_obj();
            testb = new(0) test_obj();   
            testc = new(0) test_obj();
            testa->add_dep(testb);
            testa->add_dep(testc);
        }
        while(++i < 500 ) {
            testa = new(0) test_obj();
            testb = new(0) test_obj();   
            testc = new(0) test_obj();
            testc->add_dep(testb);
            testb->add_dep();
            testa->add_dep(testc);
        }
    }
    return 0;
}