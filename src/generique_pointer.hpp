//
// Created by FAURE Adrien u on 19/03/15.
//

#ifndef _GARBAGE_COLLECTOR_GENERIQUE_POINTER_HPP_
#define _GARBAGE_COLLECTOR_GENERIQUE_POINTER_HPP_

#include <iostream>

/** \brief general behavior of a smart pointer
 */
class generique_pointer {

    public :

        /**
         * \brief default constructor 
         */
        generique_pointer() : id(generique_pointer::inc_compteur()) {};
        
        /**
         * \brief destructor
         */
        virtual ~generique_pointer() {};

        /**
         * \brief virtual fonction, implemented in smart_ptr.hpp
         */
        virtual void force_detach() = 0;

        /**
         * \brief virtual fonction, implemented in smart_ptr.hpp
         */
        virtual void * get_addr() const =  0;

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

        /**
         * \brief increment a variable to count and identify all pointers
         */
        static long inc_compteur();

private :
        /**
         */
        long static long_auto_inc;
        
        /**
         */
        long id; /**< represent identity */
};

#endif //_GARBAGE_COLLECTOR_GENERIQUE_POINTER_HPP_
