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
class garbage_collector : public IGarbageCollector {
    
    public :

        /** Get an instance of the singleton
         */
        static garbage_collector& get_instance();

        /** Brief description.
         */
        void on_attach(void *, generique_pointer);

        /** Brief description.
         */
        void on_detach(void *, generique_pointer);

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

        /** Brief description.
         */
        static garbage_collector instance;

        /** Brief description.
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
