#include <set>
#include <queue>
#include <string.h>
#include "garbage_collector.hpp"
#include <stdint.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

garbage_collector garbage_collector::instance;

garbage_collector::garbage_collector() : assoc(),
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
    if(!this->is_exist(key)) 
    {
        return  result;
    }
    for (std::set<generique_pointer*>::iterator it = this->get_out_edges(key).begin(); it != this->get_out_edges(key).end(); it++) 
    { 
        result.insert((*it)->get_addr());
    }
    return result;
}

void init_tarjan_info(tarjan_info & info, unsigned int index, unsigned int lowlink, bool onstack)
{
    info.index   = index;
    info.lowlink = lowlink;
    info.onStack = onstack;
}

/** \brief Tarjan's strongly connected components algorithm
 *  \param memories 
 *  Using Tarjan's strongly connected components algorithm, we compute 
 *   an association for each cyclic dependency to memories blocks, and we
 *   force to liberate the leaked memory
 */
void garbage_collector::TarjanAlgorithm(std::set<void *> memories)
{
    #ifdef DEBUG
        std::cerr << "void garbage_collector::TarjanAlgorithm()" << std::endl;
    #endif
    
    // TARJAN STUFF:
    unsigned int index = 0;
    
    std::stack<void *> s;
    std::vector<void *> scc;
    std::map<void *, tarjan_info> indexM;

    for ( std::set<void *>::const_iterator iter = memories.begin(); iter != memories.end(); ++iter )
    {
        if (indexM.find(*iter) == indexM.end())
        {
            std::vector<void *>  temp = this->strongconnect(*iter,index ,indexM, s);
            scc.insert(scc.end(), temp.begin(), temp.end());
        }
    }
    
    // iter over scc
    // liberate memory, use a function:
    #ifdef DEBUG
        std::cerr<< "       found "<< scc.size() << " cycles" << std::endl;
    #endif
    for ( std::vector<void *>::iterator it = scc.begin(); it != scc.end(); it++ )
    {
        std::map<void*, info_mem>::iterator map_acces = this->assoc.find(*it);
        if(map_acces != this->assoc.end()) {
            info_mem info = this->assoc.at(*it);
            std::set<generique_pointer *> ptrs = info.in;
     
            for (std::set<generique_pointer *>::iterator it3 = ptrs.begin(); it3 != ptrs.end(); it3++)
            {
                (*it3)->force_detach();
            }
        } else {
            #ifdef DEBUG
                std::cerr<< "       block " << *it << " is not in the gc" << std::endl;
            #endif
        }
    }
}

std::vector<void *> garbage_collector::strongconnect(void * v, unsigned int &index, std::map<void *, tarjan_info> &parcours_info, std::stack<void*> &stack)
{

    // Set the depth index for v to the smallest unused index
    #ifdef DEBUG
        std::cerr << "std::vector<void *> garbage_collector::strongconnect(void * "<< v <<", unsigned int &, std::map<void *, tarjan_info> &, std::stack<void*> &)" << std::endl;
    #endif
    tarjan_info node_info_init;
    init_tarjan_info(node_info_init, index, index, true);
    parcours_info.insert(std::pair<void*, tarjan_info>(v, node_info_init));
    tarjan_info &node_info = parcours_info.at(v);
    index += 1;
    stack.push(v);
    
    std::set<void *> successors = this->get_children(v);
 
    for( std::set<void *>::iterator child = successors.begin(); child != successors.end(); ++child) 
    {
        #ifdef DEBUG
            std::cerr << "  garbage_collector::strongconnect(...) in FOR LOOP, child:" << *child << std::endl;
        #endif
        if (parcours_info.find(*child) == parcours_info.end())
        {
            strongconnect(*child, index, parcours_info, stack);
            node_info.lowlink = MIN(node_info.lowlink, parcours_info.at(*child).lowlink);
        } else if (parcours_info.at(*child).onStack) {
            node_info.lowlink = MIN(node_info.lowlink, parcours_info.at(*child).index); 
        }
    }

    std::vector<void *> scc;
    if (node_info.index == node_info.lowlink) // root nod
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
        std::cerr<< "garbage_collector::~()" << std::endl;
    #endif
    this->garbage_collect();
}

/**
 */
std::set<void *> garbage_collector::dead_memoryblocks() 
{

    #ifdef DEBUG
        std::cerr << "garbage_collector::dead_memoryblocks()" << std::endl;
        std::cerr << "      there is " << this->assoc.size() << " elements stored in the gc " << std::endl;
    #endif

    std::set<void *> colored = this->coloration();
    std::set<void *> dead_memory_block;

    for (std::map<void *, info_mem>::iterator iter = this->assoc.begin(); iter != this->assoc.end(); iter++)
    {
        std::set<void *>::const_iterator found = colored.find(iter->first);
        if (found == colored.end())
        {
            #ifdef DEBUG
                std::cerr << "      found a dead block "<< *found << std::endl;
            #endif
            dead_memory_block.insert(iter->first);
        } else
        {
            #ifdef DEBUG
                std::cerr << "      found an alive block "<< *found << std::endl;
            #endif
        }
    }
    return dead_memory_block;
}

/**
 * \brief Compute a coloration of smartpointers that are accessibles from the 
 *  stack
 * \return a set of accessibles smartpointers
 */
std::set<void *> garbage_collector::coloration() 
{

    #ifdef DEBUG
        std::cerr << "garbage_collector::coloration()" << std::endl;
    #endif

    std::set<void *> colored;
    std::queue<void *> fifo;

    std::set<generique_pointer*>::const_iterator iter;
    for (iter = this->stack_pointers.begin(); iter != this->stack_pointers.end(); ++iter) 
    {
        #ifdef DEBUG
            std::cerr << "    iterate over first level stack accessible generique_pointer, pushing " << (*iter)->get_addr() << std::endl;
        #endif
        fifo.push((*iter)->get_addr());
    }

    while(!fifo.empty()) 
    {
        void * p = fifo.front();fifo.pop();
        #ifdef DEBUG
            std::cerr << "    block " << p << " can be reach from stack " << std::endl;
        #endif
    
        if (colored.find(p) == colored.end()) 
        {
            colored.insert(p);

            
            std::set<generique_pointer*> edges = this->get_out_edges(p);
            #ifdef DEBUG
                std::cerr << "    block " << p << " contains" << edges.size() << "children " << std::endl;
            #endif
            for (iter = edges.begin(); iter != edges.end(); ++iter) 
            {
                fifo.push((*iter)->get_addr());
            }
        }
    }
    #ifdef DEBUG
        std::cerr << "    colored is a set of size " << colored.size() << std::endl;
    #endif
    return colored;
}

/** return a set of accessibles smartpointer from a specific pointer
 */
void garbage_collector::on_attach(void *mem, generique_pointer &ptr)
{
    #ifdef DEBUG
        std::cerr<< "garbage_collector::on_attach() (" << mem <<")" <<std::endl;
    #endif
    
    std::map<void *, info_mem>::iterator ptrs = this->assoc.find(mem);
    if(ptrs == this->assoc.end() && mem != NULL )
    {
        #ifdef DEBUG
            std::cerr<< "       no entry for " << mem << std::endl;
        #endif
    } else
    {
        this->add_in_edges(mem, ptr);
        void * key = this->find_outer_object_of(&ptr);
        if(key == NULL) 
        {
            #ifdef DEBUG
                std::cerr<< "       find pointer from stack pointing to " << mem << std::endl;
            #endif
            this->stack_pointers.insert(&ptr);
        } else 
        {
            // key vaudra l'adresse du block m?moire auxquel appartient
            #ifdef DEBUG
                std::cerr<< "       add outgoing edge from ( " << key << ") to (" << mem <<")" << std::endl;
            #endif
            this->add_out_edges(key, ptr);
        }
    }
}

void garbage_collector::garbage_collect()
{
    #ifdef DEBUG
        std::cerr<< "garbage_collector::garbage_collect()" << std::endl;
        std::cerr<< "       there is " << this->assoc.size() << " element in the gc " << std::endl;
    #endif
    std::set<void *> dead_blocks = this->dead_memoryblocks(); 
    for(std::set<void *>::iterator it = dead_blocks.begin(); it != dead_blocks.end(); it++) {
        std::set<generique_pointer *> edges = this->get_in_edges(*it);
        for (std::set<generique_pointer *>::iterator edge = edges.begin(); edge != edges.end(); edge++)
        {
            (*edge)->force_detach();
        }
    }
    
    for(std::map<void*, info_mem >::iterator it = this->assoc.begin(); it != this->assoc.end(); it++) {
        if(!this->is_valid(it->first)) {
            free(it->first);
            //this->remove_memblock(it->first);
        }
    }
}

void garbage_collector::on_new(void * memblock, std::size_t size)
{
    #ifdef DEBUG
        std::cerr<< "garbage_collector::on_new(" << memblock << ") with size " << size << std::endl;
    #endif
    this->add_memblock(memblock);
    this->set_size(memblock, size);
}

void garbage_collector::resetInstance() 
{
    #ifdef DEBUG
        std::cerr<< "garbage_collector::resetInstance()" << std::endl;
    #endif
}

void * garbage_collector::find_outer_object_of(generique_pointer * ptr) {
    #ifdef DEBUG
        std::cerr << "void * garbage_collector::find_outer_object_of(generique_pointer * ptr) " << std::endl;
    #endif
    
    for(std::map<void *, info_mem>::iterator it = this->assoc.begin(); it != this->assoc.end(); it++) 
    {
        if(it->first <= ptr && ((uintptr_t)(it->first) + (uintptr_t)(this->get_size(it->first))) >= (uintptr_t)ptr)
        {
            #ifdef DEBUG
                std::cerr << "      pointer ("<< ptr <<") is from " << it->first << std::endl;
            #endif
            return it->first;
        }
    }
    return NULL;
}

/**
 * Our own new function to be able to use our own allocator
 * \param size the size to allocate
 * \param _ to differentiate our new to the standalone new of C++
 */
void * operator new (size_t size, int) throw (std::bad_alloc)
{
    #ifdef DEBUG
        std::cerr << "operator new(size_t )" << std::endl;
    #endif

    void *p = malloc(size);
    if (p == 0)  // did malloc succeed?
    {
        throw std::bad_alloc(); // ANSI/ISO compliant behavior
    }
    garbage_collector::get_instance().on_new(p, size);
    return p;
}

//TODO I test if memblock exist in the function, maybe it must be a requirment 
std::set<generique_pointer *>& garbage_collector::get_out_edges (void * memblock) {
    return this->assoc.at(memblock).out;
}

std::set<generique_pointer *>& garbage_collector::get_in_edges (void * memblock) {
    return this->assoc.at(memblock).in;
}

void garbage_collector::add_out_edges (void * memblock, generique_pointer &ptr) {
    if(this->is_exist(memblock)) {
        this->assoc.at(memblock).out.insert(&ptr);
    }
}

void garbage_collector::add_in_edges (void * memblock, generique_pointer &ptr) {
    if(this->is_exist(memblock)) {
        this->assoc.at(memblock).in.insert(&ptr);
    }
}

void garbage_collector::remove_out_edges (void * memblock, generique_pointer &ptr) {
    if(this->is_exist(memblock)) {
        this->assoc.at(memblock).out.erase(&ptr);
    }
}

void garbage_collector::remove_in_edges (void * memblock, generique_pointer &ptr) {
    if(this->is_exist(memblock)) {
        this->assoc.at(memblock).in.erase(&ptr);
    }
}

void garbage_collector::add_memblock(void * memblock) {
    info_mem new_info;
    this->assoc.insert(std::pair<void*, info_mem>(memblock, new_info));
}

void garbage_collector::remove_memblock(void * memblock) {
    this->assoc.erase(memblock);
}

bool garbage_collector::is_valid (void * memblock) {
    if(this->is_exist(memblock)) {
        return this->assoc.at(memblock).valid;
    }
    return false; // TODO what should we return in this case ?
}

void garbage_collector::set_valide(void * memblock, bool val) {
    if(this->is_exist(memblock)) {
        this->assoc.at(memblock).valid = val;
    } 
}

bool garbage_collector::is_exist(void * memblock) {
    return (this->assoc.find(memblock) != this->assoc.end());
}


void garbage_collector::set_size(void * memblock, std::size_t size) {
    if(this->is_exist(memblock)) {
        this->assoc.at(memblock).size = size;
    }
}

std::size_t garbage_collector::get_size (void * memblock) {
    if(this->is_exist(memblock)) {
        return this->assoc.at(memblock).size;
    } else {
        return 0;
    }
}