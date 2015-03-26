#include <set>
#include <queue>
#include "garbage_collector.hpp"
#include "smart_ptr.hpp"
#include "testing_object.hpp"

garbage_collector garbage_collector::instance;

garbage_collector &garbage_collector::get_instance() {
    return (instance);
}

garbage_collector::garbage_collector(): memblocks(), out(), assoc_size(), stack_pointers() {}

garbage_collector::~garbage_collector() {
#ifdef DEBUG
    std::cout<< "garbage_collector::~()" << std::endl;
#endif
    this->garbage_collect();
}

/**
 */
std::set<void *> garbage_collector::dead_memoryblocks() {

#ifdef DEBUG
    std::cout << "garbage_collector::dead_memoryblocks()" << std::endl;
    std::cout << "      there is " << this->memblocks.size() << " elements stored in the gc " << std::endl;
#endif

    std::set<void *> colored = this->coloration();
    std::set<void *> dead_memory_block;

    std::map<void *, std::set<generique_pointer*> >::iterator iter;
    for (iter = this->memblocks.begin(); iter != this->memblocks.end(); iter++) {

        std::set<void *>::const_iterator found = colored.find(iter->first);
        if (found == colored.end()) {
#ifdef DEBUG
            std::cout << "      found a dead block "<< *found << std::endl;
#endif
            dead_memory_block.insert(iter->first);
        } else {
#ifdef DEBUG
            std::cout << "      found an alive block "<< *found << std::endl;
#endif
        }
    }
    return dead_memory_block;
}

/** return a set of accessible smartpointer
 */
std::set<void *> garbage_collector::coloration() {

#ifdef DEBUG
    std::cout << "garbage_collector::coloration()" << std::endl;
#endif

    std::set<void *> colored;
    std::queue<void *> fifo;

    std::set<generique_pointer*>::const_iterator iter;
    for (iter = this->stack_pointers.begin(); iter != this->stack_pointers.end(); ++iter) {

#ifdef DEBUG
        std::cout << "    iterate over first level stack accessible generique_pointer, pushing " << (*iter)->get_addr() << std::endl;
#endif
        fifo.push((*iter)->get_addr());
    }

    while(!fifo.empty()) {
        void * p = fifo.front();
#ifdef DEBUG
        std::cout << "    block " << p <<" can be reach from stack " << std::endl;
#endif
        fifo.pop();
        if (colored.find(p) == colored.end()) {
            colored.insert(p);

            std::map<void *, std::set<generique_pointer*> >::iterator m = this->out.find(p);
            if (m != this->out.end()) {
#ifdef DEBUG
                std::cout << "    block " << m->first <<" contains children " << std::endl;
#endif
                std::set<generique_pointer*>::const_iterator iter;
                for (iter = m->second.begin(); iter != m->second.end(); ++iter) {
                    fifo.push((*iter)->get_addr());
                }
            }
        }
    }
#ifdef DEBUG
    std::cout << "    colored is a set of size " << colored.size() << std::endl;
#endif
    return colored;
}

/** return a set of accessibles smartpointer from a specific pointer
 */
void garbage_collector::on_attach(void *mem, generique_pointer &ptr) {
#ifdef DEBUG
    std::cout<< "garbage_collector::on_attach() (" << mem <<")" <<std::endl;
#endif
    std::map<void*, std::set<generique_pointer*> >::iterator ptrs = this->memblocks.find(mem);
    if(ptrs == this->memblocks.end() && mem != NULL ) {
#ifdef DEBUG
        std::cout<< "       no entry for " << mem << std::endl;
#endif
    } else {
        this->memblocks.at(mem).insert(&ptr);
        void * key = this->find_outer_object_of(&ptr);
        if(key == NULL) {
#ifdef DEBUG
            std::cout<< "       find pointer from stack pointing to " << mem << std::endl;
#endif
            this->stack_pointers.insert(&ptr);
        } else {
            // key vaudra l'adresse du block m?moire auxquel appartient
#ifdef DEBUG
            std::cout<< "       add outgoing edge from ( " << key << ") to (" << mem <<")" << std::endl;
#endif
            this->out.at(key).insert(&ptr);
        }
    }
}

void garbage_collector::garbage_collect()
{
#ifdef DEBUG
    std::cout<< "garbage_collector::garbage_collect()" << std::endl;
#endif
    std::set<void *> dead = this->dead_memoryblocks();
    for(std::set<void *>::iterator it = dead.begin(); it != dead.end(); it++) {
#ifdef DEBUG
        std::cout<< "       block " <<*it << " is dead, finding it over the map " << std::endl;
#endif
        std::map<void*, std::set<generique_pointer*> >::iterator outer_ptrs = this->memblocks.find(*it);
        if(outer_ptrs != this->memblocks.end()) {
            for(std::set<generique_pointer*>::iterator it2 = outer_ptrs->second.begin(); it2 != outer_ptrs->second.end(); it2++) {
                (*it2)->force_detach();
            }
        } else {
#ifdef DEBUG
            std::cout<< "       block " <<*it << " not present in garbage collector it must be an error... " << std::endl;
#endif
        }
    }
}

void garbage_collector::on_new(void * memblock, std::size_t size)
{
#ifdef DEBUG
    std::cout<< "garbage_collector::on_new(" << memblock << ") with size " << size << std::endl;
#endif
    std::set<generique_pointer*> outer_set, inner_set;

    this->memblocks.insert(std::pair<void*, std::set<generique_pointer*> >(memblock, inner_set));
    this->out.insert(std::pair<void*, std::set<generique_pointer*> >(memblock, outer_set));
    this->assoc_size.insert(std::pair<void*, std::size_t >(memblock, size));
}

void garbage_collector::resetInstance() {
#ifdef DEBUG
    std::cout<< "garbage_collector::resetInstance()" << std::endl;
#endif
}

void * garbage_collector::find_outer_object_of(const generique_pointer * ptr) {
#ifdef DEBUG
    std::cout << "void* garbage_collector::find_outer_object_of(generique_pointer *ptr) " << std::endl;
#endif
    for(std::map<void* , std::set<generique_pointer*> >::iterator it = this->memblocks.begin(); it != this->memblocks.end(); it++) {
        if(it->first <= ptr && (it->first + this->assoc_size.at(it->first)) >= ptr) {
#ifdef DEBUG
            std::cout << "      pointer ("<< ptr <<") is from " << it->first << std::endl;
#endif
            return it->first;
        }
    }
    return NULL;
}