#include <set>
#include <ctime>
#include <queue>
#include <string.h>
#include "garbage_collector.hpp"
#include <stdint.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

garbage_collector garbage_collector::instance;

garbage_collector::garbage_collector(): invalid_blocks(), lock(false), 
                                        assoc(),
                                        stack_pointers()
                                        #ifdef DEBUG 
                                            ,
                                            total_size(0),
                                            total_removed_size(0),
                                            current_size(0),
                                            invalide_size(0)
                                        #endif
{
    
}

garbage_collector &garbage_collector::get_instance() {
    return (instance);
}

std::set<void*> garbage_collector::get_children(void *key) {
    std::set<void*> result;
    if(!this->is_exist(key)) {
        return  result;
    }
    for (std::set<generique_pointer*>::iterator it = this->get_out_edges(key).begin(); it != this->get_out_edges(key).end(); it++) { 
        result.insert((*it)->get_addr());
    }
    return result;
}

void init_tarjan_info(tarjan_info & info, unsigned int index, unsigned int lowlink, bool onstack) {
    info.index   = index;
    info.lowlink = lowlink;
    info.onStack = onstack;
}

void garbage_collector::TarjanAlgorithm(std::set<void *> memories) {
    #ifdef DEBUG
        std::cerr << "void garbage_collector::TarjanAlgorithm()" << std::endl;
    #endif
    
    unsigned int index = 0;
    
    std::stack<void *> s;
    std::vector<void *> scc;
    std::map<void *, tarjan_info> indexM;

    for ( std::set<void *>::const_iterator iter = memories.begin(); iter != memories.end(); ++iter ) {
        if (indexM.find(*iter) == indexM.end()) {
            std::vector<void *>  temp = this->strongconnect(*iter,index ,indexM, s);
            scc.insert(scc.end(), temp.begin(), temp.end());
        }
    }
    
    // iter over scc
    // liberate memory, use a function:
    #ifdef DEBUG
        std::cerr<< "       found "<< scc.size() << " cycles" << std::endl;
    #endif
    for ( std::vector<void *>::iterator it = scc.begin(); it != scc.end(); it++ ) {
        std::map<void*, info_mem>::iterator map_acces = this->assoc.find(*it);
        if(map_acces != this->assoc.end()) {
            info_mem info = this->assoc.at(*it);
            std::set<generique_pointer *> ptrs = info.in;
     
            for (std::set<generique_pointer *>::iterator it3 = ptrs.begin(); it3 != ptrs.end(); it3++) {
                (*it3)->force_detach();
            }
        } else {
            #ifdef DEBUG
                std::cerr<< "       block " << *it << " is not in the gc" << std::endl;
            #endif
        }
    }
}

void garbage_collector::print_state() {
    std::map<void *, info_mem>::iterator iter;
    std::cout << std::endl << std::endl;
    std::cout << "--------------------PRINT STATE-----------------" << std::endl;
    #ifdef DEBUG
        std::cout << "--------------------DEBUG MODE-----------------" << std::endl;
        std::cout << "total size allocated   : " << this->total_size <<std::endl;
        std::cout << "total removed          : " << this->total_removed_size <<std::endl;
        std::cout << "current allocated size : " << this->current_size <<std::endl;
        std::cout << "current invalide size  : " << this->invalide_size <<std::endl;
    
    #endif
    std::cout << "-----------------------------------------------" << std::endl;
    for (iter = this->assoc.begin(); iter != this->assoc.end(); ++iter) {
        std::cout << "memblockAddr : ";
        std::cout << iter->first << std::endl;
        
        std::cout << "valid?: ";
        std::cout << ((iter->second.valid) ? "true" : "false") << std::endl;
        
        std::cout << "size: ";
        std::cout << iter->second.size << std::endl;
        
        std::set<generique_pointer *>::iterator iter2;
        std::cout << "IN : ";
        for (iter2 = iter->second.in.begin(); iter2 != iter->second.in.end(); ++iter2) {
            std::cout << *iter2 << " , ";
        }
        std::cout << std::endl ;
        
        std::cout << "OUT : ";
        for (iter2 = iter->second.out.begin(); iter2 != iter->second.out.end(); ++iter2) {
            std::cout << *iter2 << " , ";
        }
        std::cout << std::endl ;
        
        std::cout << "-----------------------------------------------" << std::endl;
    }
    std::cout << std::endl << std::endl;
} 

std::vector<void *> garbage_collector::strongconnect(void * v, unsigned int &index, std::map<void *, tarjan_info> &parcours_info, std::stack<void*> &stack) {
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
    if (node_info.index == node_info.lowlink) {
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


garbage_collector::~garbage_collector() {
    #ifdef DEBUG
        std::cerr<< "garbage_collector::~()" << std::endl;
    #endif
    this->print_state();
    this->full_garbage_collection();
    
}

/**
 */
std::set<void *> garbage_collector::dead_memoryblocks() {

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
                std::cerr << "      found a dead block "<< iter->first << std::endl;
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

std::set<void *> garbage_collector::coloration() {

    #ifdef DEBUG
        std::cerr << "garbage_collector::coloration()" << std::endl;
    #endif
    std::set<void *> colored;
    std::stack<void *> filo;

    std::set<generique_pointer*>::const_iterator iter;
    for (iter = this->stack_pointers.begin(); iter != this->stack_pointers.end(); ++iter) 
    {
        #ifdef DEBUG
            std::cerr << "    iterate over first level stack accessible generique_pointer, pushing " << (*iter)->get_addr() << std::endl;
        #endif
        filo.push((*iter)->get_addr());
    }

    while(!filo.empty()) 
    {
        void * p = filo.top(); filo.pop();
        #ifdef DEBUG
            std::cerr << "    block " << p << " can be reach from stack " << std::endl;
        #endif
    
        if (colored.find(p) == colored.end()) 
        {
            colored.insert(p);

            std::set<generique_pointer*> edges = this->get_out_edges(p);
            // p est null
            
            #ifdef DEBUG
                std::cerr << "    block " << p << " contains " << edges.size() << " children " << std::endl;
            #endif
            
            for (iter = edges.begin(); iter != edges.end(); ++iter) 
            {
                 #ifdef DEBUG
                    std::cerr << "   pushing addr : " << (*iter)->get_addr() << std::endl;
                #endif               
                filo.push((*iter)->get_addr());
            }
        }
    }
    #ifdef DEBUG
        std::cerr << "    colored is a set of size " << colored.size() << std::endl;
    #endif
    return colored;
}

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

void garbage_collector::fix_cycles(std::set<void*> dead_blocks) {
    #ifdef DEBUG
        std::cerr<< "void garbage_collector::fix_cycles(std::set<void*> dead_blocks)" << std::endl;
    #endif
    // iterate over the dead_blocks
    for(std::set<void *>::iterator it = dead_blocks.begin(); it != dead_blocks.end(); it++) {
        // iterate over input edges of each dead blocks
        std::set<generique_pointer *> edges = this->get_in_edges(*it);
        for (std::set<generique_pointer *>::iterator edge = edges.begin(); edge != edges.end(); edge++) {
           if(this->is_valid(*it)) {
                (*edge)->force_detach();
            }
        }
    }
}

int garbage_collector::small_garbage_collection() {
    std::vector<void *> free_block;
    int nb_blocks_removed = this->invalid_blocks.size();
    
    // if there is still invalids memories blocks
    while(!this->invalid_blocks.empty()) {
        void *current = this->invalid_blocks.top(); this->invalid_blocks.pop();
        #ifdef DEBUG
            this->total_removed_size += this->get_size(current);
            this->invalide_size -= this->get_size(current);
        #endif
        free(current);
        this->remove_memblock(current);
    }
    return nb_blocks_removed;
}

int garbage_collector::full_garbage_collection() {
    #ifdef DEBUG
            std::cerr<< "------begin of-----int garbage_collector::full_garbage_collection()"  << std::endl;
    #endif
    if(!this->lock) {
        this->lock = true;
        this->fix_cycles(this->dead_memoryblocks());
        #ifdef DEBUG
            int nb = this->small_garbage_collection(); 
            std::cerr<< "   "<< nb << " elements have been removed" << std::endl;
            std::cerr<< "------end of----- int garbage_collector::full_garbage_collection()"  <<std::endl;
            this->lock = false;
            return nb;
        #else
            this->lock = false;
            return this->small_garbage_collection(); 
        #endif
        } else {
            #ifdef DEBUG
                std::cerr<< "int garbage_collector::full_garbage_collection() is already running !"  << std::endl;
            #endif 
            return 0;
        }
}

void garbage_collector::on_new(void * memblock, std::size_t size)
{
    #ifdef DEBUG
        std::cerr<< "garbage_collector::on_new(" << memblock << ") with size " << size << std::endl;
        this->total_size += size;
        this->current_size += size;
    #endif
    this->add_memblock(memblock);
    this->set_size(memblock, size);
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

std::set<generique_pointer *>& garbage_collector::get_out_edges (void * memblock) {
    #ifdef DEBUG
        std::cerr << "std::set<generique_pointer *>& garbage_collector::get_out_edges (void * "<< memblock << ") " <<std::endl;
    #endif
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
    return false;
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

void * operator new (size_t size, int) throw (std::bad_alloc)
{
    #ifdef DEBUG
        std::cerr << "operator new(size_t )" << std::endl;
    #endif

    void *p = malloc(size);
    if (p == 0) { // did malloc succeed?
    
        throw std::bad_alloc(); // ANSI/ISO compliant behavior
    }
    garbage_collector::get_instance().on_new(p, size);
    return p;
}