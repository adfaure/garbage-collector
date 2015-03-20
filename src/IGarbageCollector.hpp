#ifndef _IGARBAGECOLLECTOR_
#define _IGARBAGECOLLECTOR_

#include "generique_pointer.hpp"

/**
 * \brief Class representing methods that our garbage collector need to expose
 */
class IGarbageCollector {
    public :

        /** Used when a smartpointer is filled with a pointer
         */
        virtual void on_attach(void *, generique_pointer)	  = 0;

        /** Used to detach a smartpointer to its previous element
         */
        virtual void on_detach(void *, generique_pointer)	  = 0;

        /** Informs the garbage collector that a dynamic allocation was done
         *   using inside our overloaded "new" function
         */
        virtual void on_new(void *)	  = 0;

        /** Destructor
         */
        virtual ~IGarbageCollector()  = 0;
};

#endif
