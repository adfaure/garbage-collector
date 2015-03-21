#ifndef _GARBAGE_HPP
#define _GARBAGE_HPP
#include "generique_memory_block.hpp"
#include "generique_pointer.hpp"
#include "memory_block.hpp"
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <bits/stl_bvector.h>

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
        void on_attach(void *, generique_pointer);

        /** Brief description.
         */
        template<typename T>
        void on_detach(void *mem, generique_pointer ptr) {
            #ifdef DEBUG
                std::cout<< "garbage_collector::on_detach()" << std::endl;
            #endif
            if(mem != NULL) {
                std::map<void *, std::set<generique_pointer> >::iterator ptrs = this->memblocks.find(mem);
                if (ptrs == this->memblocks.end()) {
                #ifdef DEBUG
                    std::cout << "	no entry for " << mem << std::endl;
                #endif
                } else {
                    this->memblocks.at(mem).erase(ptr);
                    #ifdef DEBUG
                    std::cout << "	no pointer on (" << mem << ")... deleting " << std::endl;
                    #endif
                    if (this->memblocks.at(mem).empty()) {
                        // sorting block version
                        memory_block<T> *mem_block = new memory_block<T>(mem);
                        this->lost_blocks.push_back(mem_block);

                        // classical version
                        //this->memblocks.erase(mem);
                        //delete static_cast<T *>(mem);


                    }
                }
            }
        }

        /** Brief description.
         */
        void on_new(void *);

        /** Brief description.
         */
        static void resetInstance();

    private :

        /** Brief description.
         */
        void garbage_collect();

        /** instance of the singleton
         */
        static garbage_collector instance;

        /**
         *
         */
        std::vector<generique_memory_block*> lost_blocks; // TODO is that ok to use pointers? i would rather use references but it does not work

        /** Associate memory block to the smartpointers that use the membock
         */
        std::map<void* , std::set<generique_pointer> > memblocks;

        /** Private constructor to ensure the singleton pattern
         */
        garbage_collector();
        
        /** Destructor
         */
        ~garbage_collector();
};

#endif
