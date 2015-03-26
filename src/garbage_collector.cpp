#include <set>
#include <queue>
#include <stack>
#include <string.h>
#include <strings.h>
#include "garbage_collector.hpp"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

garbage_collector garbage_collector::instance;

garbage_collector::garbage_collector() : memblocks(), out(), assoc_size(), stack_pointers() {}

garbage_collector &garbage_collector::get_instance() {
    return (instance);
}

std::set<void*> garbage_collector::get_children(void *key) {
    std::set<void*> result;
    if(this->out.find(key) == this->out.end()) {
        return  result;
    }
    for(std::set<generique_pointer*>::iterator it = this->out.at(key).begin(); it != this->out.at(key).end(); it++) {
        result.insert((*it)->get_addr());
    }
    return result;
}

void garbage_collector::TarjanAlgorithm() {
#ifdef DEBUG
    std::cout << "void garbage_collector::TarjanAlgorithm()" << std::endl;
#endif
    unsigned int compteur = 0;
    std::map<void *, std::pair<unsigned int,unsigned int> > colored;
    std::stack<void *> filo, last_parcour;

    for(std::map<void *, std::set<generique_pointer*> >::iterator node = this->memblocks.begin(); node != this->memblocks.end(); node++) {
        if(colored.find(node->first) == colored.end()) {
            filo.push(node->first);
            while(!filo.empty()) {
                void * current_node = filo.top(); filo.pop();
                last_parcour.push(current_node);

                std::pair<unsigned int, unsigned int> index(compteur, compteur); compteur++;
                std::set<void *> children = this->get_children(current_node);

                for(std::set<void *>::iterator child = children.begin(); child != children.end(); child++) {
                    std::map<void *, std::pair<unsigned int,unsigned int> >::iterator node_children = colored.find(*child);
                    if(node_children == colored.end()) {
                        filo.push(*child);
                    } else {
                        std::pair<unsigned int, unsigned int> child_index = node_children->second;
                        index.second = MIN(child_index.first, index.first);
                     }
                }

                std::pair<void *, std::pair<unsigned int, unsigned int> > node(current_node, index);
                colored.insert(std::pair<void *, std::pair<unsigned int, unsigned int> >(current_node, index));
            }
#ifdef DEBUG
            std::cout << "      -----------------------------------------------------------------" << std::endl;
#endif
            while(!last_parcour.empty()) {

                void * current_node = last_parcour.top(); last_parcour.pop();
                std::pair<unsigned int, unsigned int> index = colored.at(current_node);

                std::set<void *> children = this->get_children(current_node);
                for(std::set<void *>::iterator child = children.begin(); child != children.end(); child++) {
                    std::map<void *, std::pair<unsigned int,unsigned int> >::iterator node_children = colored.find(*child);
                    if(node_children == colored.end()) {
                        filo.push(*child);
                    } else {
                        std::pair<unsigned int, unsigned int> child_index = node_children->second;
                        index.second = MIN(child_index.first, index.first);
                    }
                }
            }
        }
    }
    for(std::map<void *, std::pair<unsigned int,unsigned int> >::iterator it = colored.begin(); it != colored.end(); it++) {
        std::cout<< it->first << "  " << it->second.first << " " << it->second.second << " ";
    }
}

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
    std::cout<< "       ther is " << this->memblocks.size() << " element in the gc " << std::endl;
#endif
    this->TarjanAlgorithm();
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