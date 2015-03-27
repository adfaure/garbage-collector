#ifndef _GARBAGE_HPP
#define _GARBAGE_HPP
#include "generique_memory_block.hpp"
#include "generique_pointer.hpp"
#include "memory_block.hpp"
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include <bits/stl_bvector.h>

#include <cstdlib> // for malloc() and free()

typedef struct S_tarjan_info {
    unsigned int index, lowlink;
    bool onStack;
} tarjan_info;

void init_tarja_info(tarjan_info & info, unsigned int index, unsigned int lowlink, bool onstack);


/**
 * \brief This class represent our garbage collector
 */
class garbage_collector {

public :

    /** Get an instance of the singleton
     */
    static garbage_collector& get_instance();

    /** Brief description.
     */
    void on_attach(void *, generique_pointer &);

    /** Brief description.
     */
    template<typename T>
    void on_detach(void *mem, generique_pointer &ptr)
    {
        #ifdef DEBUG
            std::cout<< "garbage_collector::on_detach() sur le block " << mem << std::endl;
        #endif

        std::set<generique_pointer*>::iterator stack_ptr = this->stack_pointers.find(&ptr);
        if(stack_ptr != this->stack_pointers.end()) 
        {
            #ifdef DEBUG
                std::cout<< "       stack pointer on (" << mem <<") removed from stack_set "<< std::endl;
            #endif
            this->stack_pointers.erase(&ptr);
        }

        if(mem != NULL) 
        {
            std::map<void *, std::set<generique_pointer*> >::iterator ptrs = this->memblocks.find(mem);
            if (ptrs == this->memblocks.end())
            {
                #ifdef DEBUG
                    std::cout << "	no entry for : " << mem << std::endl;
                #endif
            } else
            {
#ifdef DEBUG
                std::cout << "	there is " << this->memblocks.at(mem).size() <<" pointer on "<< mem << std::endl;
#endif
                this->memblocks.at(mem).erase(&ptr);

                if (this->memblocks.at(mem).empty())
                {
                    #ifdef DEBUG
                        std::cout << "	no pointer on (" << mem << ")... deleting " << std::endl;
                    #endif
                    this->memblocks.erase(mem);
                    this->out.erase(mem);
                    this->assoc_size.erase(mem);

                    delete static_cast<T *>(mem);
                    // TODO : the three codes lines below can replace the delete and avoid mismatch operator warning in valgrind
                    /*
                    T *elem_cast = static_cast<T *>(mem);
                    elem_cast->~T();
                    free(mem);
                    */
                }
            }
        } else 
        {
            #ifdef DEBUG
                std::cout << "	NULL " << mem << std::endl;
            #endif
        }
    }

    /**
    */
    std::set<void *> dead_memoryblocks();

    /** return a set of acessible smartpointer
     */
    std::set<void *> coloration();

    /** Brief description.
     */
    void on_new(void *, std::size_t );

    /** Brief description.
     */
    void resetInstance();

private :

    /**
    *
    */
    void TarjanAlgorithm();
    std::vector<void *>  strongconnect(void * v, unsigned int &index, std::map<void *, tarjan_info> &parcours_info, std::stack<void*> &stack);

    /*
    *
    *
    */
    std::set<void*> get_children(void *key);

    /** Brief description.
     */
    void garbage_collect();

    /** instance of the singleton
     */
    static garbage_collector instance;

    /** \brief return null if ptr is not attached to a memory block
    *
    */
    void * find_outer_object_of(const generique_pointer *);

    /** Associate memory block to the smartpointers that use the membock
     */
    std::map<void *, std::set<generique_pointer*> > memblocks;

    /** Associate memory block to the smartpointers
    */
    std::map<void *, std::set<generique_pointer*> > out;

    /**
    *
    */
    std::map<void* , std::size_t > assoc_size;

    /**
    *
    */
    std::set<generique_pointer*> stack_pointers;

    /** Private constructor to ensure the singleton pattern
     */
    garbage_collector();

    /** Destructor
     */
    ~garbage_collector();
};

void* operator new (std::size_t size, int bidon) throw (std::bad_alloc);

#endif
