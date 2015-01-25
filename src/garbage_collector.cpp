#include "garbage_collector.hpp"

garbage_collector *garbage_collector::instance = 0;

garbage_collector &garbage_collector::get_instance() {
    if(!instance) {
        instance = new garbage_collector();
    }
    return (*instance);
}


garbage_collector::~garbage_collector() {
    delete instance;
}