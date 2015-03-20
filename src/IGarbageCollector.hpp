#ifndef _IGARBAGECOLLECTOR_
#define _IGARBAGECOLLECTOR_

#include "generique_pointer.hpp"

class IGarbageCollector {
	public :

		virtual void on_attach(void *, generique_pointer) = 0;
		virtual void on_detach(void *, generique_pointer) = 0;
		virtual void on_new(void *)	  = 0;
		virtual ~IGarbageCollector()  = 0;
};

#endif 
