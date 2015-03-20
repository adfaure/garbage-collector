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

        /**
         * \brief Brief description.
         */
        static garbage_collector& get_instance();

        /**
         * \brief Brief description.
         */
        void on_attach(void *, generique_pointer);

        /**
         * \brief Brief description.
         */
        void on_detach(void *, generique_pointer);

        /**
         * \brief Brief description.
         */
        void on_new(void *);

        /**
         * \brief Brief description.
         */
        static void resetInstance();
    
    private : 

        /**
         * \brief Brief description.
         */
        void free_all();

        /**
         * \brief Brief description.
         */
        static garbage_collector instance;

        /**
         * \brief Brief description.
         */
        std::map<void *, std::set<generique_pointer> > memblocks;

        /**
         * \brief Brief description.
         */
        garbage_collector();
        
        /**
         * \brief Brief description.
         */
        ~garbage_collector();
};

#endif
