#include "garbage_collector.hpp"

garbage_collector garbage_collector::instance;

garbage_collector &garbage_collector::get_instance() {
    return (instance);
}

garbage_collector::~garbage_collector() {
	#ifdef DEBUG
		std::cout<< "~garbage_collector()" << std::endl;
	#endif
}
		
void garbage_collector::on_attach() {
	#ifdef DEBUG
		std::cout<< "on_attach()" << std::endl;
	#endif
}

void garbage_collector::on_detach() {
	#ifdef DEBUG
		std::cout<< "on_detach()" << std::endl;
	#endif
}


void garbage_collector::resetInstance() {
	#ifdef DEBUG
		std::cout<< "resetInstance()" << std::endl;
	#endif
}
