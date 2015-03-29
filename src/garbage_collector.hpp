#ifndef _GARBAGE_HPP
#define _GARBAGE_HPP

#include <bits/stl_bvector.h>
#include <iostream>
#include <cstdlib> // for malloc() and free()
#include <vector>
#include <stack>
#include <map>
#include <set>

#include "generique_pointer.hpp"

/**
 * Store informations we use on the computation of the Tarjan's Algorithm
 */
typedef struct S_tarjan_info tarjan_info;

/**
 * Store informations about a specific memoryblock
 */
typedef struct S_info_mem info_mem;

/**
 */
void init_tarjan_info(tarjan_info & info, unsigned int index, unsigned int lowlink, bool onstack);

/** overloaded new for the garbage_collector the whole programm must use new(0) now.
 */
void * operator new (std::size_t size, int bidon) throw (std::bad_alloc);


/**
 * \brief This class represent our garbage collector
 */
class garbage_collector {

public :

    /** Get an instance of the singleton
     */
    static garbage_collector& get_instance();

    /** Register a smart_pointer to the garbage collector
     */
    void on_attach(void *, generique_pointer &);

    /** Unregister a smart_pointer to the garbage collector
     */
    template<typename T>
    void on_detach(void *mem, generique_pointer &ptr);

    /**
     *  
     */
    template<typename T>
    void intern_on_detach(void *mem, generique_pointer &ptr);

    /**
     */
    void on_new(void *, std::size_t );

    /**
     */
    void resetInstance();

private :

    /**
     * 
     */
    std::stack<void *> invalid_blocks;

    /**
     */ 
    bool lock;
    
    /**
     */
    std::map<void *, info_mem> assoc;

    /** Static instance of the singleton 
     */
    static garbage_collector instance;
    
    /** Brief description.
     */
    void garbage_collect();

    void print_state();

    /** Get the children of a memoryblock
     */
    std::set<void*> get_children(void *key);

    /** Returns a set of memoryblock that are not accessible from anywhere in stack
     */
    std::set<void *> dead_memoryblocks();

    /**
     * \brief 
     */
     void fix_cycles(std::set<void *> dead_blocks);

    /**
     * \brief Small garbage collection
     * \return the number of free-ed blocks
     * We do a small garbage collection to free invalids memories blocks.
     * An invalid memory block is a memory block that have destructor 
     *  called on it but not yet freed.
     */
    int small_garbage_collection();

    /**
     * \brief Full garbage collection
     * \return the number of free-ed blocks
     */
    int full_garbage_collection();

    /** Returns a set of memoryblock that are accessible from anywhere in stack
     */
    std::set<void *> coloration();

    /** \brief return null if ptr is not attached to a memory block
     */
    void * find_outer_object_of(generique_pointer *);

    /** \brief Tarjan's strongly connected components algorithm
    *  \param memories
    *  Using Tarjan's strongly connected components algorithm, we compute
    *   an association for each cyclic dependency to memories blocks, and we
    *   force to liberate the leaked memory
    */
    void TarjanAlgorithm(std::set<void *> memories);

    /**
     */
    std::vector<void *> strongconnect(void * v, unsigned int &index, std::map<void *, tarjan_info> &parcours_info, std::stack<void*> &stack);

    /**
     * Collection of smartpointers
     */
    std::set<generique_pointer*> stack_pointers;

    /**
     * 
     */
    std::set<generique_pointer *>& get_out_edges (void * memblock);

    /**
     */
    std::set<generique_pointer *>& get_in_edges (void * memblock);

    /**
     */
    void add_out_edges (void * memblock, generique_pointer &ptr);

    /**
     */
    void add_in_edges (void * memblock, generique_pointer &ptr);

    /**
     */
    void add_memblock(void * memblock);

    /**
     */
    void remove_memblock(void * memblock);

    /**
     */
    void remove_out_edges (void * memblock, generique_pointer &ptr);

    /**
     */
    void remove_in_edges (void * memblock, generique_pointer &ptr);

    /**
     */
    void set_size(void * memblock, std::size_t );

    /**
     */
    std::size_t get_size (void * memblock);

    /**
     */
    bool is_valid (void * memblock);

    /**
     */
    void set_valide(void * memblock, bool val);

    /**
     */
    bool is_exist(void * memblock);

    /** Private constructor to ensure the singleton pattern
     */
    garbage_collector();

    /** Destructor
     */
    ~garbage_collector();
    
#ifdef DEBUG
    
    unsigned long total_size;
    
    unsigned long total_removed_size;
    
    unsigned long current_size;
    
    unsigned long invalide_size;
    
#endif 
};

template<typename T>
void garbage_collector::on_detach(void *mem, generique_pointer &ptr) {
    #ifdef DEBUG
        std::cerr<< "garbage_collector::on_detach() sur le block " << mem << std::endl;
    #endif
    
    std::set<generique_pointer*>::iterator stack_ptr = this->stack_pointers.find(&ptr);
    if(stack_ptr != this->stack_pointers.end()) {
        #ifdef DEBUG
            std::cerr<< "       stack pointer on (" << mem <<") removed from stack_set "<< std::endl;
        #endif
        this->stack_pointers.erase(&ptr);
    }
    
    if(mem != NULL) {
        if (!this->is_exist(mem)) {
            // this should not be happend
            #ifdef DEBUG
                std::cerr << "	no entry for : " << mem << std::endl;
            #endif
        } else {
            
            // remove the generic pointer from the map 
            this->remove_in_edges(mem, ptr);
            #ifdef DEBUG
                std::cerr << "	there is " << this->get_in_edges(mem).size() << " pointer attaching to "<< mem << std::endl;
            #endif

            if (this->get_in_edges(mem).empty() && this->is_valid(mem)) {
                #ifdef DEBUG
                    std::cerr << "	no pointer left on (" << mem << ") ... destructor will be called " << std::endl;
                    this->invalide_size += this->get_size(mem);
                #endif
                this->set_valide(mem, false);
                this->invalid_blocks.push(mem);
                T *elem_cast = static_cast<T *>(mem);
                elem_cast->~T();
            } else {
                full_garbage_collection();
            }
        }
    }
}

struct S_tarjan_info {
    unsigned int index, lowlink;
    bool onStack;
};

/**
 * Store informations about a specific memoryblock
 */
struct S_info_mem {
    public :
        S_info_mem() : in(), out(), size(0), valid(true) {};
        std::set<generique_pointer *> in;
        std::set<generique_pointer *> out;
        std::size_t                   size;
        bool                          valid;
};

#endif

