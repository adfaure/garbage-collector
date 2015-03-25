#include "garbage_collector.hpp"

garbage_collector garbage_collector::instance;

garbage_collector &garbage_collector::get_instance() {
    return (instance);
}

garbage_collector::garbage_collector(): memblocks(),  assoc_size(), stack_pointers() {}

garbage_collector::~garbage_collector() {
#ifdef DEBUG
    std::cout<< "garbage_collector::~()" << std::endl;
#endif
    this->garbage_collect();
}


void garbage_collector::on_attach(void *mem, generique_pointer &ptr) {
#ifdef DEBUG
    std::cout<< "garbage_collector::on_attach() (" << mem <<")" <<std::endl;
#endif
    std::map<void*, std::set<generique_pointer> >::iterator ptrs = this->memblocks.find(mem);
    if(ptrs == this->memblocks.end() && mem != NULL ) {
#ifdef DEBUG
        std::cout<< "       no entry for " << mem << std::endl;
#endif
    } else {
        this->memblocks.at(mem).insert(ptr);
        void * key = this->find_inner_object(&ptr);
        if(key == NULL) {
#ifdef DEBUG
            std::cout<< "       find stack pointer to " << mem << std::endl;
#endif
            this->stack_pointers.insert(ptr);
        } else {

        }
    }
}

void garbage_collector::garbage_collect()
{
#ifdef DEBUG
    std::cout<< "garbage_collector::garbage_collect()" << std::endl;
#endif
}

void garbage_collector::on_new(void * memblock, std::size_t size)
{
#ifdef DEBUG
    std::cout<< "garbage_collector::on_new(" << memblock << ") with size " << size << std::endl;
#endif
    std::set<generique_pointer> outer_set, inner_set;

    this->memblocks.insert(std::pair<void*, std::set<generique_pointer> >(memblock, outer_set));
    this->assoc_size.insert(std::pair<void*, std::size_t >(memblock, size));
}

void garbage_collector::resetInstance() {
#ifdef DEBUG
    std::cout<< "garbage_collector::resetInstance()" << std::endl;
#endif
}

void* garbage_collector::find_inner_object(generique_pointer *ptr) {
#ifdef DEBUG
    std::cout << "void* garbage_collector::find_inner_object(generique_pointer *ptr) " << std::endl;
#endif
    for(std::map<void* , std::set<generique_pointer> >::iterator it = this->memblocks.begin(); it != this->memblocks.end(); it++) {
        if(it->first <= ptr && (it->first + this->assoc_size.at(it->first)) >= ptr) {
#ifdef DEBUG
            std::cout << "      pointer ("<< ptr <<") is from " << it->first << std::endl;
#endif
            return it->first;
        }
    }
    return NULL;
}