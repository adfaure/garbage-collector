#ifndef _smart_ptr_
#define _smart_ptr_

#include <new>
#include <iostream>
#include <exception> // for std::bad_alloc
#include <cstdlib> // for malloc() and free()
#include "generique_pointer.hpp"
#include "IGarbageCollector.hpp"
#include "garbage_collector.hpp"

/**
 * \brief smartPointer class
 */ 
template<typename T>
class smart_ptr : public generique_pointer {

    public :

        /**
         * \brief Constructor
         *
        */
        smart_ptr(T* var_elem = NULL) : generique_pointer(),
                                        elem(var_elem),
                                        garbage(garbage_collector::get_instance()) {
            #ifdef DEBUG
                std::cout << "smart_ptr(T* elem = NULL)" << std::endl;
            #endif

            if(this->elem != NULL) {
            #ifdef DEBUG
                std::cout << "	smart_ptr initializing to ("<< var_elem <<") (used on attach)" << std::endl;
            #endif
                this->garbage.on_attach((void *) var_elem, *(this));
            } else {
            #ifdef DEBUG
                std::cout << "	smart_ptr initializing to NULL " << std::endl;
            #endif
            }
        };

        /**
        * \brief Copy Constructor
        *
        */
        smart_ptr(const smart_ptr & rhs) : generique_pointer(),
                                           elem(rhs.elem),
                                           garbage(garbage_collector::get_instance()) // TODO copy semantic ?
        {
            #ifdef DEBUG
                std::cout << "smart_ptr(const smart_ptr &)" << std::endl;
            #endif
            if(this->elem != NULL) {
            #ifdef DEBUG
                std::cout << "	smart_ptr initializing to ("<< rhs.elem <<") (used on attach)" << std::endl;
            #endif
                this->garbage.on_attach((void *) rhs.elem, *(this));
            } else {
            #ifdef DEBUG
                std::cout << "	smart_ptr initializing to NULL " << std::endl;
            #endif
            }
        };

        /**
         * \brief destructor
         */
        ~smart_ptr() {
            #ifdef DEBUG
                std::cout << "~smart_ptr()" << std::endl;
            #endif
            elem = NULL;
        };

        /**
         * \brief overload of operator = in case of acces to element
         *
         */
        smart_ptr<T> &operator =(T *var_elem) {
            #ifdef DEBUG
                std::cout<< "smart_ptr operator =(elem *) "<< var_elem << std::endl;
            #endif

            if(this->elem != NULL) {
            #ifdef DEBUG
                std::cout << " detaching smart_ptr detaching to its previous element" << std::endl;
            #endif
                this->garbage. template on_detach<T>(this->elem, *(this));
            }

            if(var_elem != NULL ) {
            #ifdef DEBUG
                std::cout << "	smart_ptr attaching to element"<< std::endl;
            #endif
                this->garbage.on_attach((void *) var_elem , *(this));
            }

            elem = var_elem;
            return (*this);
        };

        /**
         * \brief overload operator = in case of affectation to another smart pointers
         *
         */
        smart_ptr<T> &operator =(const smart_ptr<T> &ptr)  {
            #ifdef DEBUG
                std::cout<< "smart_ptr operator =(const smart_ptr<T> &ptr)" << std::endl;
            #endif

            if(this->elem != NULL) {
            #ifdef DEBUG
                std::cout << " detaching smart_ptr detaching to its previous element" << std::endl;
            #endif
                this->garbage. template on_detach<T>(this->elem, *(this));
            }

            if(ptr.elem != NULL ) {
            #ifdef DEBUG
                std::cout << "	smart_ptr attaching to element"<< std::endl;
            #endif
                this->garbage.on_attach((void *) ptr.elem, *(this));
            }

            //normal affectation
            this->elem = ptr.elem;
            return (*this);
        };

        /**
         * \brief deferencing element to accès element
         *
         */
        T &operator *() const {
            #ifdef DEBUG
                std::cout<< "smart_ptr operator*() " << elem << std::endl;
            #endif
            return (*elem);
        };

        /** Overload arrow operator to member access
         */
        T *operator->() const {
            #ifdef DEBUG
                std::cout<< "smart_ptr operator->() " << std::endl;
            #endif
            return elem;
        };

        /** Egality operator
         */
        inline bool operator==(const T* r_member) {
            return r_member == this->elem;
        };

    private :

        T *elem; /**< pointer on the content of the smartpointer */

        garbage_collector &garbage; /**< the singleton instance of the        */
                                    /**< garbage collector                    */
};

void* operator new (std::size_t size, int bidon) throw (std::bad_alloc);

#endif 
