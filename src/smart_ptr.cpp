#include "smart_ptr.hpp"

void* operator new (size_t size, int bidon) throw (std::bad_alloc)
{
	#ifdef DEBUG
		std::cout<< "oerator new(size_t ) " << std::endl;
	#endif	
	void *p=malloc(size); 
	if (p==0) // did malloc succeed?
		throw std::bad_alloc(); // ANSI/ISO compliant behavior

	return p;
}
