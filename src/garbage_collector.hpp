#ifndef _GARBAGE_HPP
#define _GARBAGE_HPP

/**
 * 
 */
class garbage_collector {
    
    public :
        /**
         * 
         */
        static garbage_collector& get_instance();
    
    private : 
        
        /**
         * 
         */
        static garbage_collector* instance;
        
        /**
         * 
         */
        garbage_collector() {};
        
        /**
         * 
         */
        ~garbage_collector();
};
    

#endif