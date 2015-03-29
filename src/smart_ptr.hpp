#ifndef _smart_ptr_
#define _smart_ptr_

#include <new>
#include <iostream>
#include <vector>
#include <exception> // for std::bad_alloc

#include "garbage_collector.hpp"

/**
 * \brief Our Smart pointer implementation
 */
template<typename T>
class smart_ptr : public generique_pointer {

public :

    /**
     * \brief Construct a new smartpointer pointing to NULL
     */
    smart_ptr();

    /**
     * Construct a new smartpointer based on an existing smartpointer
     * \param rhs an existing smartpointer we want to copy
     *
     * Note that the copy semantic we choose is to share the element pointed by
     *  the rhs smartpointer with the newly created smartpointer
     */
    smart_ptr(const smart_ptr & rhs);

    /**
     * Construct a new smartpointer based on an existing pointer on T
     * \param var_element a pointer on i T instance
     */
    smart_ptr(T* var_elem);

    /**
     * \brief Destruct the SmartPointer, and notify the garbage collector
     */
    ~smart_ptr();
    
    /**
     * \brief Overload of operator = in case of accessing to element
     */
    smart_ptr<T> &operator =(T *var_elem);

    /**
     * \brief overload operator = in case of affectation to another smart pointers
     */
    smart_ptr<T> &operator =(const smart_ptr<T> &ptr) ;

    
    /**
     * \brief Deferencing element to access element
     */
    virtual T & operator *() const;
    
    /** 
     * \brief Overload arrow operator to member access on the pointed element
     */
    T *operator->() const ;
    
    /** 
     * \brief Egality operator
     */
    inline bool operator==(const T* r_member);

    /**
     * \brief Overload [] operator to provide array-like access allowing 
     *        both reading and writing
     */
    T &operator[](std::size_t idx);
    
    /**
     * \brief Overload [] operator to provide array-like access allowing 
     *        both reading
     */
    const T &operator[](std::size_t idx) const;

    /** 
     * \brief Virtual function from @generique_pointer, 
     *        this function gets the adress of pointed element as void*
     */
    void * get_addr() const;

    /** 
     * \brief Virtual function from @generique_pointer, 
     *        force the pointer to quit his object
     */
    virtual void force_detach();

    /** 
     * \brief Overload to get the adress of pointed element
     */
    template<typename X>
    friend std::ostream& operator<<(std::ostream &os, const smart_ptr<X> &ptr);

private :

    T *elem; /**< pointer on the content of the smartpointer */

    garbage_collector &garbage; /**< the singleton instance of the GC */
};

template<typename T>
smart_ptr<T>::smart_ptr() :  generique_pointer(), elem(NULL), garbage(garbage_collector::get_instance()) {}

template<typename T>
smart_ptr<T>::smart_ptr(T* var_elem) :  generique_pointer(),
                                    elem(var_elem),
                                    garbage(garbage_collector::get_instance())
{
    #ifdef DEBUG
            std::cerr << "smart_ptr(T* elem = "<< var_elem <<")" << std::endl;
    #endif

    if(this->elem != NULL) {
        #ifdef DEBUG
            std::cerr << "	smart_ptr initializing to ("<< var_elem <<") (used on attach)" << std::endl;
        #endif
        this->garbage.on_attach((void *) var_elem, *(this));
    } else {
        #ifdef DEBUG
            std::cerr << "	smart_ptr initializing to NULL " << std::endl;
        #endif
    }
}

template<typename T>
T& smart_ptr<T>::operator[](std::size_t idx) { 
    #ifdef DEBUG
        std::cerr<< "T& smart_ptr<T>::operator[](std::size_t" << idx <<")" << std::endl;
    #endif
    return (this->elem)[idx];
}

template<typename T>
const T& smart_ptr<T>::operator[](std::size_t idx) const {
    #ifdef DEBUG
        std::cerr<< "const T& smart_ptr<T>::operator[](std::size_t" << idx <<") const" << std::endl;
    #endif
    return (this->elem)[idx];
}

template<typename T>
smart_ptr<T>::smart_ptr(const smart_ptr & rhs) : generique_pointer(),
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
    } else {
        #ifdef DEBUG
            std::cerr << "	smart_ptr initializing to NULL " << std::endl;
        #endif
    }
}

/**
 * \brief Destruct the spartpointer, and notify the garbage collector
 */
 template<typename T>
smart_ptr<T>::~smart_ptr()
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
}

template<typename T>
smart_ptr<T> &smart_ptr<T>::operator =(T *var_elem) 
{
    #ifdef DEBUG
        std::cerr<< "-----------------smart_ptr operator = (T = " << var_elem << ") " << std::endl;
    #endif
    T *temp = elem;
    elem = var_elem;
    if(temp != NULL) 
    {
        #ifdef DEBUG
            std::cerr << "      detaching smart_ptr to its previous element" << std::endl;
        #endif
        this->garbage. template on_detach<T>(temp, *(this));
    }

    if(var_elem != NULL ) 
    {
        #ifdef DEBUG
            std::cerr << "      smart_ptr attaching to element"<< std::endl;
        #endif
        this->garbage.on_attach((void *) var_elem , *(this));
    }
    return (*this);
}


template<typename T>
smart_ptr<T> &smart_ptr<T>::operator =(const smart_ptr<T> &ptr)  
{
    #ifdef DEBUG
        std::cerr<< "smart_ptr operator =(const smart_ptr<T> &ptr)" << std::endl;
    #endif
    T *temp = elem;
    elem = ptr.elem;
    if(temp != NULL)
    {
        #ifdef DEBUG
            std::cerr << "detaching smart_ptr to its previous element" << std::endl;
        #endif
        this->garbage. template on_detach<T>(temp, *(this));
    }

    if(ptr.elem != NULL )
    {
        #ifdef DEBUG
            std::cerr << "	smart_ptr attaching to element"<< std::endl;
        #endif
        this->garbage.on_attach((void *) ptr.elem, *(this));
    }

    //normal affectation
    return (*this);
}

template<typename T>
T & smart_ptr<T>::operator *() const
{
    #ifdef DEBUG
        std::cerr<< "smart_ptr operator*() " << elem << std::endl;
    #endif
    return (*elem);
}

template<typename T>
T *smart_ptr<T>::operator->() const 
{
    #ifdef DEBUG
        std::cerr<< "smart_ptr operator->() " << std::endl;
    #endif
    return elem;
}

template<typename T>
bool smart_ptr<T>::operator==(const T* r_member) 
{
    return r_member == this->elem;
}

template<typename T>
void * smart_ptr<T>::get_addr() const 
{
    void * ans = static_cast<void *>(this->elem);
    #ifdef DEBUG
        std::cerr<< "void * smart_ptr::get_addr() const : " << ans <<  std::endl;
    #endif
    return ans;
}

template<typename T>
void smart_ptr<T>::force_detach()  
{
    #ifdef DEBUG
        std::cerr << "virtual void force_detach() on " << this << std::endl;
    #endif

    if(this->elem != NULL)
    {
        this->garbage. template on_detach<T>(elem, *(this));
    }
}

template<typename T>
std::ostream& operator<<(std::ostream &os, const smart_ptr<T> &ptr) {
    os << ptr.elem ;
    return os;
}


#endif 
