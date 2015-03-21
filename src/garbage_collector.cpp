#include "garbage_collector.hpp"

garbage_collector garbage_collector::instance;

garbage_collector &garbage_collector::get_instance() {
    return (instance);
}

garbage_collector::garbage_collector(): lost_blocks(), memblocks() {}

garbage_collector::~garbage_collector() {
    #ifdef DEBUG
        std::cout<< "garbage_collector::~()" << std::endl;
    #endif
    this->garbage_collect();
}

void garbage_collector::on_attach(void *mem, generique_pointer ptr) {
    #ifdef DEBUG
        std::cout<< "garbage_collector::on_attach()" << std::endl;
    #endif
    std::map<void*, std::set<generique_pointer> >::iterator ptrs = this->memblocks.find(mem);
    if(ptrs == this->memblocks.end()) {
        #ifdef DEBUG
                std::cout<< "aucune entrée pour dans le garbage collector" << mem << std::endl;
        #endif
    } else {
        this->memblocks.at(mem).insert(ptr);
    }
}

void garbage_collector::garbage_collect()
{
    #ifdef DEBUG
        std::cout<< "garbage_collector::garbage_collect()" << std::endl;
    #endif

     for(std::vector<generique_memory_block>::iterator it = this->lost_blocks.begin() ; it != this->lost_blocks.end(); it++) {
        it->destroy(); // FIXME : why base class function is calling here , should be the derived class because the function destroy() is virtual
    }
}

void garbage_collector::on_new(void * memblock)
{
    #ifdef DEBUG
        std::cout<< "garbage_collector::on_new(" << memblock << ")" << std::endl;
    #endif
    std::set<generique_pointer> set; // TODO i think this line is weird, do we have a better issue to insert the set in ourm ap?
    this->memblocks.insert(std::pair<void*, std::set<generique_pointer> >(memblock, set));
}

void garbage_collector::resetInstance() {
    #ifdef DEBUG
        std::cout<< "garbage_collector::resetInstance()" << std::endl;
    #endif
}
