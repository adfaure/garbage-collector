#include <set>
#include <queue>
#include <stack>
#include <string.h>
#include <strings.h>
#include "garbage_collector.hpp"
#include <algorithm> 

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

garbage_collector garbage_collector::instance;

garbage_collector::garbage_collector() : memblocks(),
                                         out(), 
                                         assoc_size(), 
                                         stack_pointers()
{
    
}

garbage_collector &garbage_collector::get_instance() 
{
    return (instance);
}

std::set<void*> garbage_collector::get_children(void *key)
{
    std::set<void*> result;
    if(this->out.find(key) == this->out.end()) 
    {
        return  result;
    }
    for(std::set<generique_pointer*>::iterator it = this->out.at(key).begin(); it != this->out.at(key).end(); it++) 
    {
        result.insert((*it)->get_addr());
    }
    return result;
}

void init_tarja_info(tarjan_info & info, unsigned int index, unsigned int lowlink, bool onstack)
{
    info.index   = index;
    info.lowlink = lowlink;
    info.onStack = onstack;
}

void garbage_collector::TarjanAlgorithm() 
{
    #ifdef DEBUG
        std::cout << "void garbage_collector::TarjanAlgorithm()" << std::endl;
    #endif
    
    // TARJAN STUFF:
    unsigned int index = 0;
    
    std::stack<void *> s;
    std::vector<void *> scc;
    std::map<void *, tarjan_info> indexM;
    
    for ( std::map<void *, std::set<generique_pointer *> >::const_iterator iter = this->memblocks.begin(); iter != this->memblocks.end(); ++iter )
    {
        if (indexM.find(iter->first) == indexM.end())
        {
            std::vector<void *>  temp = this->strongconnect(iter->first,index ,indexM, s);
            scc.insert(scc.end(), temp.begin(), temp.end());
        }
    }
    
    // iter over scc
    for ( std::vector<void *>::iterator it = scc.begin(); it != scc.end(); it++ )
    {
        #ifdef DEBUG
            std::cout<< "-----------------------------scc-----------------------------" << std::endl;
        #endif
        std::cout << "*it : " << *it << std::endl;
        std::map<void*, std::set<generique_pointer*> >::iterator map_acces = this->memblocks.find(*it);
        if(map_acces != this->memblocks.end()) {
            std::set<generique_pointer *> ptrs = this->memblocks.at(*it);
            for (std::set<generique_pointer *>::iterator it3 = ptrs.begin(); it3 != ptrs.end(); it3++)
            {
                if((*it3)->isPtrValide()) {
                    (*it3)->force_detach();
                }
            }
        } else {
#ifdef DEBUG
            std::cout<< "       block " << *it << " is not in the gc" << std::endl;
#endif
        }
    }
#ifdef DEBUG
    std::cout<< "-----------------------------end-----------------------------" << std::endl;
#endif
}

std::vector<void *> garbage_collector::strongconnect(void * v, unsigned int &index, std::map<void *, tarjan_info> &parcours_info, std::stack<void*> &stack)
{

    // Set the depth index for v to the smallest unused index

    tarjan_info node_info_init;
    init_tarja_info(node_info_init, index, index, true);
    parcours_info.insert(std::pair<void*, tarjan_info>(v, node_info_init));
    tarjan_info &node_info = parcours_info.at(v);
    
    index += 1;
    stack.push(v);
    
    std::set<void *> successors = this->get_children(v);
    for( std::set<void *>::iterator child = successors.begin(); child != successors.end(); ++child) 
    {
        if (parcours_info.find(*child) == parcours_info.end())
        {
            strongconnect(*child, index, parcours_info, stack);
            node_info.lowlink = MIN(node_info.lowlink, parcours_info.at(*child).lowlink);
        } else if (parcours_info.at(*child).onStack) {
            node_info.lowlink = MIN(node_info.lowlink, parcours_info.at(*child).index); 
        }
    }

    std::vector<void *> scc;
    if (node_info.index == node_info.lowlink) 
    {
        scc.push_back(stack.top());
        void *current_node;
        do 
        {
            current_node = stack.top(); stack.pop();
            parcours_info.at(current_node).onStack = false;
            //add w to current strongly connected component

        } while(v != current_node);
    }
    return scc;
}


garbage_collector::~garbage_collector() 
{
    #ifdef DEBUG
        std::cout<< "garbage_collector::~()" << std::endl;
    #endif
    this->garbage_collect();
}

/**
 */
std::set<void *> garbage_collector::dead_memoryblocks() 
{

    #ifdef DEBUG
        std::cout << "garbage_collector::dead_memoryblocks()" << std::endl;
        std::cout << "      there is " << this->memblocks.size() << " elements stored in the gc " << std::endl;
    #endif

    std::set<void *> colored = this->coloration();
    std::set<void *> dead_memory_block;

    std::map<void *, std::set<generique_pointer*> >::iterator iter;
    for (iter = this->memblocks.begin(); iter != this->memblocks.end(); iter++)
    {
        std::set<void *>::const_iterator found = colored.find(iter->first);
        if (found == colored.end())
        {
            #ifdef DEBUG
                std::cout << "      found a dead block "<< *found << std::endl;
            #endif
            dead_memory_block.insert(iter->first);
        } else
        {
            #ifdef DEBUG
                std::cout << "      found an alive block "<< *found << std::endl;
            #endif
        }
    }
    return dead_memory_block;
}

/** return a set of accessible smartpointer
 */
std::set<void *> garbage_collector::coloration() 
{

    #ifdef DEBUG
        std::cout << "garbage_collector::coloration()" << std::endl;
    #endif

    std::set<void *> colored;
    std::queue<void *> fifo;

    std::set<generique_pointer*>::const_iterator iter;
    for (iter = this->stack_pointers.begin(); iter != this->stack_pointers.end(); ++iter) 
    {
        #ifdef DEBUG
            std::cout << "    iterate over first level stack accessible generique_pointer, pushing " << (*iter)->get_addr() << std::endl;
        #endif
        fifo.push((*iter)->get_addr());
    }

    while(!fifo.empty()) 
    {
        void * p = fifo.front();
        #ifdef DEBUG
            std::cout << "    block " << p <<" can be reach from stack " << std::endl;
        #endif
        fifo.pop();
        if (colored.find(p) == colored.end()) 
        {
            colored.insert(p);

            std::map<void *, std::set<generique_pointer*> >::iterator m = this->out.find(p);
            if (m != this->out.end()) 
            {
                #ifdef DEBUG
                    std::cout << "    block " << m->first <<" contains children " << std::endl;
                #endif
                std::set<generique_pointer*>::const_iterator iter;
                for (iter = m->second.begin(); iter != m->second.end(); ++iter) 
                {
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
void garbage_collector::on_attach(void *mem, generique_pointer &ptr)
{
    #ifdef DEBUG
        std::cout<< "garbage_collector::on_attach() (" << mem <<")" <<std::endl;
    #endif
    std::map<void*, std::set<generique_pointer*> >::iterator ptrs = this->memblocks.find(mem);
    if(ptrs == this->memblocks.end() && mem != NULL )
    {
        #ifdef DEBUG
            std::cout<< "       no entry for " << mem << std::endl;
        #endif
    } else
    {
        this->memblocks.at(mem).insert(&ptr);
        void * key = this->find_outer_object_of(&ptr);
        if(key == NULL) 
        {
            #ifdef DEBUG
                std::cout<< "       find pointer from stack pointing to " << mem << std::endl;
            #endif
            this->stack_pointers.insert(&ptr);
        } else 
        {
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
        std::cout<< "       there is " << this->memblocks.size() << " element in the gc " << std::endl;
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

void garbage_collector::resetInstance() 
{
    #ifdef DEBUG
        std::cout<< "garbage_collector::resetInstance()" << std::endl;
    #endif
}

void * garbage_collector::find_outer_object_of(const generique_pointer * ptr) {
    #ifdef DEBUG
        std::cout << "void* garbage_collector::find_outer_object_of(generique_pointer *ptr) " << std::endl;
    #endif
    for(std::map<void* , std::set<generique_pointer*> >::iterator it = this->memblocks.begin(); it != this->memblocks.end(); it++) 
    {
        if(it->first <= ptr && (it->first + this->assoc_size.at(it->first)) >= ptr)
        {
            #ifdef DEBUG
                std::cout << "      pointer ("<< ptr <<") is from " << it->first << std::endl;
            #endif
            return it->first;
        }
    }
    return NULL;
}