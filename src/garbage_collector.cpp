#include "garbage_collector.hpp"

garbage_collector garbage_collector::instance;

garbage_collector &garbage_collector::get_instance() {
    return (instance);
}

garbage_collector::garbage_collector(): IGarbageCollector(), memblocks() {}

garbage_collector::~garbage_collector() {
	#ifdef DEBUG
		std::cout<< "garbage_collector::~()" << std::endl;
	#endif
	this->free_all();
}
		
void garbage_collector::on_attach() {
	#ifdef DEBUG
		std::cout<< "garbage_collector::on_attach()" << std::endl;
	#endif
}

void garbage_collector::on_detach() {
	#ifdef DEBUG
		std::cout<< "garbage_collector::on_detach()" << std::endl;
	#endif
}

void garbage_collector::free_all()
{
	#ifdef DEBUG
		std::cout<< "garbage_collector::free_all()" << std::endl;
	#endif
	for (std::vector<void*>::iterator it = this->memblocks.begin(); it != this->memblocks.end(); it++)
	{
		 delete *it;
	}
}

void garbage_collector::on_new(void * memblock)
{
	#ifdef DEBUG
		std::cout<< "garbage_collector::on_new(" << memblock << ")" << std::endl;
	#endif
	this->memblocks.push_back(memblock);
}

void garbage_collector::resetInstance() {
	#ifdef DEBUG
		std::cout<< "garbage_collector::resetInstance()" << std::endl;
	#endif
}
