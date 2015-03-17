#ifndef _IGARBAGECOLLECTOR_
#define _IGARBAGECOLLECTOR_

class IGarbageCollector {
	public :
		virtual void on_attach()	  = 0;
		virtual void on_detach()	  = 0; 
		//virtual void register_ptr()   = 0;
		//virtual void unregister_ptr() = 0;
		virtual void on_new(void *)		  = 0;
		virtual ~IGarbageCollector()  = 0;
};

#endif 
