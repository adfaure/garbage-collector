//
// Created by dadou on 19/03/15.
//

#ifndef _GARBAGE_COLLECTOR_GENERIQUE_POINTER_HPP_
#define _GARBAGE_COLLECTOR_GENERIQUE_POINTER_HPP_

#include <iostream>

/** \brief general behavior of a smart pointer
 */
class generique_pointer {

    public :
        friend class garbage_collector;

        /** \brief return egality on generics pointers on their id attribute
         */
        inline bool operator ==(const generique_pointer &rhs) const {
                return (this->id == rhs.id);
        };

        /** \brief compare two generics pointers on their id attributes
         *
         */
        inline bool operator <(const generique_pointer &rhs) const {
                return (this->id < rhs.id);
        };

private :

        long id; /**< represent TODO */
};

#endif //_GARBAGE_COLLECTOR_GENERIQUE_POINTER_HPP_
