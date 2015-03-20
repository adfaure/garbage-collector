//
// Created by dadou on 19/03/15.
//

#ifndef _GARBAGE_COLLECTOR_GENERIQUE_POINTER_HPP_
#define _GARBAGE_COLLECTOR_GENERIQUE_POINTER_HPP_

#include <iostream>

class generique_pointer {

    public :
        friend class garbage_collector;

        /**
        * \brief return egaliti on genrique pointers on their id attribute
        */
        inline bool operator ==(const generique_pointer &rhs) const {
        #ifdef DEBUG
                std::cout << "bool operator ==(const generique_pointer &rhs)" << std::endl;
        #endif
                return (this->id == rhs.id);
        };

        /**
        * \brief comapre two generique pointers on their id attribute
        *
        */
        inline bool operator <(const generique_pointer &rhs) const {
        #ifdef DEBUG
                std::cout << "bool operator <(const generique_pointer &rhs)" << std::endl;
        #endif
                return (this->id < rhs.id);
        };

private :

        /**
        *
        */
        long id;
};

#endif //_GARBAGE_COLLECTOR_GENERIQUE_POINTER_HPP_
