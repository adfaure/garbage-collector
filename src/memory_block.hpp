//
// Created by dadou on 21/03/15.
//

#ifndef _GARBAGE_COLLECTOR_MEMORY_BLOCK_HPP_
#define _GARBAGE_COLLECTOR_MEMORY_BLOCK_HPP_

#include "generique_memory_block.hpp"

template<typename  T>
class memory_block : public generique_memory_block {
    public :
    
        memory_block(void *elem) : generique_memory_block(), ptr(static_cast<T*>(elem)) {};

        memory_block(const memory_block<T> &mem) : generique_memory_block(), ptr(mem.ptr) {};

        bool operator =(const memory_block<T> &lhs) {
            return (this->ptr == lhs.ptr);
        };

    virtual void destroy() {
        #ifdef DEBUG
            std::cout<< "destroy() memory block " << ptr << " (derived class)" << std::endl;
        #endif
            delete ptr;
        };

        ~memory_block() {
        };

    private:
        T *ptr;
};

#endif //_GARBAGE_COLLECTOR_MEMORY_BLOCK_HPP_
