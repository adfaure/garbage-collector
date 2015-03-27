#ifndef _generique_memory_block_
#define _generique_memory_block_

#include <iostream>

class generique_memory_block {
    public :

    virtual void destroy() {
            #ifdef DEBUG
                std::cerr<< "detroy on base class" << std::endl;
            #endif
        }

    virtual ~generique_memory_block(){}
};

#endif