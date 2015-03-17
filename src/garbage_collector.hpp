#ifndef _GARBAGE_HPP
#define _GARBAGE_HPP

#include "IGarbageCollector.hpp"
#include "IGarbageCollector.hpp"
#include <iostream>
#include <vector>

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
		void on_attach();
		
		/**
		 *
		 */
		void on_detach();

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
        std::vector<void *> memblocks;

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
