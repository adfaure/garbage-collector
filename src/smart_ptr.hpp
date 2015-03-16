#ifndef _smart_ptr_
#define _smart_ptr_

#include <new>
#include "garbage_collector.hpp"
#include <exception> // for std::bad_alloc
#include <cstdlib> // for malloc() and free()
/**
 * \brief smartPointer class
 */ 
template<typename T>
class smart_ptr {
	
	public :

		/**
		 * \brief Constructor
		 *
		 */
		smart_ptr() : elem(NULL), garbage(garbage_collector::get_instance()) {
			#ifdef DEBUG
				std::cout << "smart_ptr()" << std::endl;  
			#endif 
		};

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
			elem = var_elem;
			return (*this);
		};
		
		/**
		 * \brief overload operator = in case of affectation to another smart pointers
		 *
		 */
		smart_ptr<T> &operator =(smart_ptr<T> ptr)  {
			#ifdef DEBUG 
				std::cout<< "smart_ptr operator =(smart_ptr)" << std::endl;  
			#endif
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
		
		/**
		 * \brief overload arrow operator to member accès
		 *
		 */ 
		T *operator->() const {
			#ifdef DEBUG
				std::cout<< "smart_ptr operator->() " << std::endl;
			#endif
			return elem;
		};

		inline bool operator==(const T* r_member) {
			return r_member == this->elem;
		};

	private :

		/**
		 * \brief element 
		 */
		T *elem;

		/**
		 *
		 */
		IGarbageCollector &garbage;
};

void* operator new (std::size_t size, int bidon)  throw (std::bad_alloc);

#endif 
