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
 * \deprecated {Use @fix_cycles instead} Tarjan algo is not used in the final version 
 * Store informations we use on the computation of the Tarjan's Algorithm
 */
typedef struct S_tarjan_info tarjan_info;

/**
 * \brief initialize the struct that contains usefull informations for the 
 * Tarjan algo
 */
void init_tarjan_info(tarjan_info & info, unsigned int index, unsigned int lowlink, bool onstack);

/**
 * \brief Store informations about a specific memoryblock 
 * (used to the garbage collector to keep information of all allocated blocks)
 */
typedef struct S_info_mem info_mem;


/** 
 * \brief Overloaded new for the garbage_collector the whole programm must use new(0).
 * \param size the size to allocate
 * \param b this param is just here to differentiate our new with the 
 *          builtin new of C++
 */
void * operator new (std::size_t size, int b) throw (std::bad_alloc);

/**
 * \brief This class represent our garbage collector
 */
class garbage_collector {

public :

    /** 
     * \brief Get the instance of the singleton
     */
    static garbage_collector& get_instance();

    /**
     * \brief Register an association smart_pointer to memoryblock
     *        on the garbage collector
     * \param mem the memory block used by the smartpointer
     * \param ptr the smartpointer
     */
    void on_attach(void *mem, generique_pointer &ptr);

    /** 
     * \brief Unregister an association smart_pointer to memoryblock to the garbage collector
     * \param mem 
     * \param ptr 
     */
    template<typename T>
    void on_detach(void *mem, generique_pointer &ptr);

    /**
     * \brief Notify the garbage collector of a new allocated blocks.
     * \param void * the adress of the new element
     * \param std::size_t the size of the new block
     */
    void on_new(void * memblock, std::size_t size);

private :

    /**
     * \brief Collection of memory blocks that are unreachable (dead), 
     *        destructed (invalid), but not yet freed
     */
    std::stack<void *> invalid_blocks;
    
    /**
     * \brief Represent if the full GC is running or not
     */ 
    bool lock;
    
    /** 
     * \brief Store informations about a specific memoryblock
     */
    std::map<void *, info_mem> assoc;

    /** 
     * \brief Static instance of the singleton 
     */
    static garbage_collector instance;
    
    /**
     * Collection of smartpointers
     */
    std::set<generique_pointer*> stack_pointers;

    /**
     * \brief debug function, allow to print on the output the state of the garbage collector
     */
    void print_state();

    /** Get the children of a memoryblock
     * \param void * key the elemnts to get children
     * \return std::set<void*> of all children as void*.
     */
    std::set<void*> get_children(void *key);

    /** 
     * \brief Returns a set of memoryblock that are not accessible from anywhere in stack
     * \return std::set<void*> of all dead memory blocks
     */
    std::set<void *> dead_memoryblocks();

    /**
     * \brief resolve cycles depencies by forcing all smart_pointer to detach to an element.
     * \param std::set<void *> dead_blocks the block to freeing
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

    /** \brief Return a set of memoryblock that are accessible from anywhere in stack
     * \return std::set<void *> a set of all blocks reachable form the stack
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
     * \deprecated used for tarjan algorithm
     */
    std::vector<void *> strongconnect(void * v, unsigned int &index, std::map<void *, tarjan_info> &parcours_info, std::stack<void*> &stack);

    /**
     * \brief find all pointers owned by the void*
     * \param void * the key of the object
     */
    std::set<generique_pointer *>& get_out_edges (void * memblock);

    /**
     * \brief  find all pointers pointing to the void* passed in parameter
     * \param void* the key
     * \return std::set<generique_pointer *>& a collection of all smart pointers refering the key
     */
    std::set<generique_pointer *>& get_in_edges (void * memblock);

    /**
     * \brief add a new outgoing edge
     * \param the memory blocks
     * \param a referenc to the pointer
     */
    void add_out_edges (void * memblock, generique_pointer &ptr);

   /**
     * \brief add a new incoming edge
     * \param the memory blocks
     * \param a referenc to the pointer
     */
    void add_in_edges (void * memblock, generique_pointer &ptr);

    /**
     * \brief a dd a new memblock to the garbage collector
     */
    void add_memblock(void * memblock);

    /**
     * \brief remove a memblock to the garbage collector
     */
    void remove_memblock(void * memblock);

    /**
     * \brief remove outgoing edge to the key
     * \param void* 
     * \param generique_pointer &
     */
    void remove_out_edges (void * memblock, generique_pointer &ptr);

    /**
     * \brief remove incoming edge to the key
     * \param void* 
     * \param generique_pointer &
     */
    void remove_in_edges (void * memblock, generique_pointer &ptr);

    /**
     * \brief set the size of a memory block
     * \param void * the memory block
     * \param std::size_t the size
     */
    void set_size(void * memblock, std::size_t );

    /**
     * \brief the size of the void * passed in parameters
     * \paramvoid * the addr of the memoryblock
     */
    std::size_t get_size (void * memblock);

    /**
     * \brief this function return true if a memory block is invalide (the destructor have been call)
     * \param void* the key to get this information
     */
    bool is_valid (void * memblock);

    /**
     * \brief change the validity of an address (false means destructor have been call)
     * \param void * the address
     * \param bool the value
     */
    void set_valide(void * memblock, bool val);

    /**
     * \brief return true if the garbage collector know the adress passed in parameters
     * \param void * 
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

struct S_info_mem {
    public :
        S_info_mem() : in(), out(), size(0), valid(true) {};
        std::set<generique_pointer *> in;
        std::set<generique_pointer *> out;
        std::size_t                   size;
        bool                          valid;
};

#endif

