#include <iostream>
#include "garbage_collector.hpp"
#include "smart_ptr.hpp"
#include "testing_object.hpp"

#define POINTEUR(T) smart_ptr<T>

int main() {

    POINTEUR(test_obj) test = new(1) test_obj();

    return 0;
}
