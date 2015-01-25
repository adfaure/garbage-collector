#include <iostream>
#include "garbage_collector.hpp"
int main() {
    std::cout << "hello world \n";
    garbage_collector &singleton = garbage_collector::get_instance();
    
    #ifdef DEBUG
        std::cout<< "mode debug [on] \n";
    #endif
}