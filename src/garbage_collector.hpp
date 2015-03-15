#ifndef _GARBAGE_HPP
#define _GARBAGE_HPP

#include "IGarbageCollector.hpp"
#include <iostream> 

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
    
    private : 
        
        /**
         * 
         */
        static garbage_collector *instance;
        
        /**
         * 
         */
        garbage_collector() : IGarbageCollector() {};
        
        /**
         * 
         */
        ~garbage_collector();
};
    

#endif
