#ifndef _smart_ptr_
#define _smart_ptr_

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
		smart_ptr() : elem(NULL) {
			#ifdef DEBUG
				std::cout << "smart_ptr()" << std::endl;  
			#endif 
		};

		/** 
		 * \brief overload of operator = in case of acces to element
		 *
		 */
		void operator =(T *var_elem) {
			 #ifdef DEBUG
				std::cout<< "smart_ptr operator =(elem *) "<< var_elem << std::endl;
			 #endif
			elem = var_elem;
		};
		
		/**
		 * \brief overload operator = in case of affectation to another smart pointers
		 *
		 */
		void operator =(smart_ptr<T> ptr)  {
			#ifdef DEBUG 
				std::cout<< "smart_ptr operator =(smart_ptr)" << std::endl;  
			#endif
			this->elem = ptr.elem; 
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
};
#endif 
