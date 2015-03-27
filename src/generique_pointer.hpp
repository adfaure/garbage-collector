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

        generique_pointer() : isValide(true), id(generique_pointer::inc_compteur()) {};

        virtual ~generique_pointer() {};

        virtual void force_detach() = 0;

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

        virtual void * get_addr() const {
            return NULL;
        };

        static long inc_compteur();

        bool isPtrValide() {
#ifdef DEBUG
            std::cout << "bool isvalide() " << isValide << std::endl;
#endif
            return isValide;
        }

        mutable bool isValide;
private :
        long static long_auto_inc;
        long id; /**< represent TODO */
};

#endif //_GARBAGE_COLLECTOR_GENERIQUE_POINTER_HPP_
