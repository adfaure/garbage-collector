#ifndef _smart_ptr_
#define _smart_ptr_

#include <new>
#include <iostream>
#include <exception> // for std::bad_alloc
#include <cstdlib> // for malloc() and free()
#include "generique_pointer.hpp"

#include "garbage_collector.hpp"

/**
 * \brief smartPointer class
 */
template<typename T>
class smart_ptr : public generique_pointer
{

public :

    /** Construct a new smartpointer pointing to nothing
     */
    smart_ptr(T* var_elem = NULL) : generique_pointer(),
                                    elem(var_elem),
                                    garbage(garbage_collector::get_instance()) 
    {
        #ifdef DEBUG
                std::cerr << "smart_ptr(T* elem = NULL)" << std::endl;
        #endif

        if(this->elem != NULL) {
            #ifdef DEBUG
                std::cerr << "	smart_ptr initializing to ("<< var_elem <<") (used on attach)" << std::endl;
            #endif
            this->garbage.on_attach((void *) var_elem, *(this));
        } else
        {
            #ifdef DEBUG
                std::cerr << "	smart_ptr initializing to NULL " << std::endl;
            #endif
        }
    };

    /**
     * Construct a new smartpointer based on an existing smartpointer
     * \param rhs an existing smartpointer we want to copy
     *
     * Note that the copy semantic we choose is to share the element pointed by
     *  the rhs smartpointer with the newly created smartpointer
     */
    smart_ptr(const smart_ptr & rhs) : generique_pointer(),
                                       elem(rhs.elem),
                                       garbage(garbage_collector::get_instance())
    {
        #ifdef DEBUG
            std::cerr << "smart_ptr(const smart_ptr &)" << std::endl;
        #endif

        if(this->elem != NULL)
        {
            #ifdef DEBUG
                std::cerr << "	smart_ptr initializing to ("<< rhs.elem <<") (used on attach)" << std::endl;
            #endif
            this->garbage.on_attach((void *) rhs.elem, *(this));
        } else
        {
            #ifdef DEBUG
                std::cerr << "	smart_ptr initializing to NULL " << std::endl;
            #endif
        }
    };

    /**
     * \brief Destruct the spartpointer, and notify the garbage collector
     */
    ~smart_ptr()
    {
        #ifdef DEBUG
            std::cerr << "~smart_ptr() addr :  " << this << std::endl;
        #endif

        if (this->elem != NULL) {
            #ifdef DEBUG
                std::cerr << "     detaching to " << elem << std::endl;
            #endif
            this->garbage.template on_detach<T>(elem, *(this));
            this->elem = NULL;
        }

    };

    /**
     * \brief overload of operator = in case of accessing to element
     */
    smart_ptr<T> &operator =(T *var_elem) 
    {
        #ifdef DEBUG
            std::cerr<< "smart_ptr operator =(elem *) "<< var_elem << std::endl;
        #endif

        if(this->elem != NULL) 
        {
            #ifdef DEBUG
                std::cerr << "      detaching smart_ptr to its previous element" << std::endl;
            #endif
            this->garbage. template on_detach<T>(this->elem, *(this));
        }

        if(var_elem != NULL ) 
        {
            #ifdef DEBUG
                std::cerr << "      smart_ptr attaching to element"<< std::endl;
            #endif
            this->garbage.on_attach((void *) var_elem , *(this));
        }

        elem = var_elem;
        return (*this);
    };


    /**
     * \brief overload operator = in case of affectation to another smart pointers
     */
    smart_ptr<T> &operator =(const smart_ptr<T> &ptr)  
    {
        #ifdef DEBUG
            std::cerr<< "smart_ptr operator =(const smart_ptr<T> &ptr)" << std::endl;
        #endif

        if(this->elem != NULL)
        {
            #ifdef DEBUG
                std::cerr << "detaching smart_ptr to its previous element" << std::endl;
            #endif
            this->garbage. template on_detach<T>(this->elem, *(this));
        }

        if(ptr.elem != NULL )
        {
            #ifdef DEBUG
                std::cerr << "	smart_ptr attaching to element"<< std::endl;
            #endif
            this->garbage.on_attach((void *) ptr.elem, *(this));
            //this->garbage. template on_detach<T>((void *) ptr.elem, *(this));
        }

        //normal affectation
        this->elem = ptr.elem;
        return (*this);
    };

    /**
     * \brief deferencing element to access element
     */
    virtual T & operator *() const
    {
        #ifdef DEBUG
            std::cerr<< "smart_ptr operator*() " << elem << std::endl;
        #endif
        return (*elem);
    };

    /** Overload arrow operator to member access on the pointed element
     */
    T *operator->() const 
    {
        #ifdef DEBUG
            std::cerr<< "smart_ptr operator->() " << std::endl;
        #endif
        return elem;
    };

    /** Egality operator
     */
    inline bool operator==(const T* r_member) 
    {
        return r_member == this->elem;
    };

    void * get_addr() const 
    {
        void * ans = static_cast<void *>(this->elem);
        #ifdef DEBUG
            std::cerr<< "void * smart_ptr::get_addr() const : " << ans <<  std::endl;
        #endif
        return ans;
    }

    virtual void force_detach()  
    {
        #ifdef DEBUG
            std::cerr << "virtual void force_detach() on " << this << std::endl;
        #endif

        if(this->elem != NULL)
        {
            this->garbage. template on_detach<T>(elem, *(this));
        }
    };

    friend std::ostream& operator<<(std::ostream &os, const smart_ptr<T> &ptr) {
        os << ptr.elem ;
        return os;
    };

private :

    T *elem; /**< pointer on the content of the smartpointer */

    garbage_collector &garbage; /**< the singleton instance of the GC */
};


#endif 
