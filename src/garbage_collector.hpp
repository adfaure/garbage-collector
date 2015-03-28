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
 */
typedef struct S_tarjan_info tarjan_info;

/**
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
     */
    void on_new(void *, std::size_t );

    /**
     */
    void resetInstance();

private :

    /* Get the children of a memoryblock
     */
    std::set<void*> get_children(void *key);

    /** Brief description.
     */
    void garbage_collect();

    /** instance of the singleton
     */
    static garbage_collector instance;

    /** Returns a set of memoryblock that are not accessible from anywhere in stack
     */
    std::set<void *> dead_memoryblocks();

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
    std::vector<void *>  strongconnect(void * v, unsigned int &index, std::map<void *, tarjan_info> &parcours_info, std::stack<void*> &stack);

    /**
     */
    std::map<void *, info_mem> assoc;

    /**
     */
    std::set<generique_pointer*> stack_pointers;

    /**
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
};

template<typename T>
void garbage_collector::on_detach(void *mem, generique_pointer &ptr)
{
    #ifdef DEBUG
        std::cerr<< "garbage_collector::on_detach() sur le block " << mem << std::endl;
    #endif

    std::set<generique_pointer*>::iterator stack_ptr = this->stack_pointers.find(&ptr);
    if(stack_ptr != this->stack_pointers.end())
    {
        #ifdef DEBUG
            std::cerr<< "       stack pointer on (" << mem <<") removed from stack_set "<< std::endl;
        #endif
        this->stack_pointers.erase(&ptr);
    }

    if(mem != NULL)
    {
        if (!this->is_exist(mem))
        {
            #ifdef DEBUG
                std::cerr << "	no entry for : " << mem << std::endl;
            #endif
        } else {
            this->remove_in_edges(mem, ptr);
            #ifdef DEBUG
                std::cerr << "	there is " << this->get_in_edges(mem).size() <<" pointer on "<< mem << std::endl;
            #endif

            if (this->get_in_edges(mem).empty() && this->is_valid(mem))
            {
                #ifdef DEBUG
                    std::cerr << "	no pointer on (" << mem << ")... deleting " << std::endl;
                #endif

                // delete static_cast<T *>(mem);
                // TODO : the three codes lines below can replace the delete and avoid mismatch operator warning in valgrind

                this->set_valide(mem, false);
                T *elem_cast = static_cast<T *>(mem);
                elem_cast->~T();
                
                // free(mem);
            }
        }
    } else
    {
        #ifdef DEBUG
            std::cerr << "	NULL " << mem << std::endl;
        #endif
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

