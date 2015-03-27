#include "smart_ptr.hpp"

/**
 * Our own new function to be able to use our own allocator
 * \param size the size to allocate
 * \param _ to differentiate our new to the standalone new of C++
 */
void * operator new (size_t size, int) throw (std::bad_alloc)
{
    #ifdef DEBUG
        std::cout<< "operator new(size_t ) " << std::endl;
    #endif

    void *p = malloc(size);
    if (p == 0)  // did malloc succeed?
    {
        throw std::bad_alloc(); // ANSI/ISO compliant behavior
    }
    garbage_collector::get_instance().on_new(p, size);
    return p;
}
