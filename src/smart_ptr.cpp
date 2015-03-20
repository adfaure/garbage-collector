#include "smart_ptr.hpp"

/**
* \brief
* \parame
*/
void* operator new (size_t size, int) throw (std::bad_alloc)
{
	#ifdef DEBUG
		std::cout<< "operator new(size_t ) " << std::endl;
	#endif
	void *p = malloc(size);
	if (p == 0) // did malloc succeed?
	{
		throw std::bad_alloc(); // ANSI/ISO compliant behavior
	}
	garbage_collector::get_instance().on_new(p);
	return p;
}