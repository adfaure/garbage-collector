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

        virtual void on_new(void *)	  = 0;
        virtual ~IGarbageCollector()  = 0;
};

#endif
