#ifndef __RAINDROP_UTILS_WRAPPERS_HPP__
#define __RAINDROP_UTILS_WRAPPERS_HPP__

#include "common.hpp"

namespace Raindrop::Utils{
	template<typename Context_t, typename T, typename ID_t, ID_t defaultID = (ID_t)(~0)>
	class Wrapper{
		public:
			Wrapper() : _context{nullptr}, _ID{defaultID}{}
			Wrapper(const Wrapper& other) : _context{other._context}, _ID{other._ID}{}
			Wrapper(Context_t& context, const ID_t& ID) : _context{&context}, _ID{ID}{}

			virtual ~Wrapper() = default;

			Wrapper& operator=(const Wrapper& other){
				_context = other._context;
				_ID = other._ID;
				return *this;
			}

			Wrapper& operator=(const ID_t& ID){
				_ID = ID;
				return *this;
			}

			virtual T& get() = 0;
			virtual const T& get() const = 0;

			T* operator->(){
				return &get();
			}

			const T* operator->() const{
				return &get();
			}

			ID_t ID() const{
				return _ID;
			}

		protected:
			Context_t* _context;
			ID_t _ID;
	};
}

#endif