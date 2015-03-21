#ifndef _GARBAGE_HPP
#define _GARBAGE_HPP

#include "IGarbageCollector.hpp"
#include "generique_pointer.hpp"
#include <iostream>
#include <map>
#include <set>

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
            std::map<void*, std::set<generique_pointer> >::iterator ptrs = this->memblocks.find(mem);
            if(ptrs == this->memblocks.end()) {
                #ifdef DEBUG
                    std::cout<< "	aucune entré pour dans le garbage collector" << mem << std::endl;
                #endif
            } else {
                this->memblocks.at(mem).erase(ptr);
                #ifdef DEBUG
                    std::cout<< "	no pointer on (" << mem <<")... deleting " << std::endl;
                #endif
                if(this->memblocks.at(mem).empty()) {
                    this->memblocks.erase(mem);
                    delete static_cast<T *>(mem);
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
        void free_all();

        /** instance of the singleton
         */
        static garbage_collector instance;

        /** Associate memory block to the smartpointers that use the membock
         */
        std::map<void *, std::set<generique_pointer> > memblocks;

        /** Private constructor to ensure the singleton pattern
         */
        garbage_collector();
        
        /** Destructor
         */
        ~garbage_collector();
};

#endif
