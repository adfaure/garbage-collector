#ifndef _GARBAGE_HPP
#define _GARBAGE_HPP

#include "IGarbageCollector.hpp"
#include "generique_pointer.hpp"
#include <iostream>
#include <map>
#include <set>

/**
 * 
 */
class garbage_collector : public IGarbageCollector {
    
    public :

        /**
         * 
         */
        static garbage_collector& get_instance();

		/**
		 * \brief 
		 *
		 */
		void on_attach(void *, generique_pointer);
		
		/**
		 *
		 */
		void on_detach(void *, generique_pointer);

        /**
         *
         */
        void on_new(void *);

		/**
		 *
		 */
		static void resetInstance();
    
    private : 

        /**
         *
         */
        void free_all();

        /**
         * 
         */
        static garbage_collector instance;

        /**
         *
         */
        std::map<void *, std::set<generique_pointer> > memblocks;

        /**
         * 
         */
        garbage_collector();
        
        /**
         * 
         */
        ~garbage_collector();
};

#endif
