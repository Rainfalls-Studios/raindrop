#ifndef __RAINDROP_CORE_MEMORY_GUARDED_POINTER_HPP__
#define __RAINDROP_CORE_MEMORY_GUARDED_POINTER_HPP__

#include "common.hpp"

namespace Raindrop::Core::Memory{
	template<typename T>
	class GuardedPtr{
		public:
			GuardedPtr() : _count{nullptr}, _refPointer{nullptr}{}
			GuardedPtr(const GuardedPtr& other) : _count{other._count}, _refPointer{other._refPointer}{
				increment();
			}

			~GuardedPtr(){
				reset();
			}

			GuardedPtr& operator=(const GuardedPtr& other){
				_count = other._count;
				_refPointer = other._refPointer;
				increment();
				return *this;
			}

			void reset(){
				if (decrement() == 0){
					if (assetAlive()){
						*_refPointer = nullptr;
						*_count = 0;

						delete _count;
						delete _refPointer;
					}
					_count = nullptr;
					_refPointer = nullptr;
				}
			}

			void release(){
				if (_count != nullptr){
					_count = 0;
					
					delete *_refPointer;
					*_refPointer = nullptr;
				}
			}

			T* get() const noexcept{
				return *_refPointer;
			}

			T& operator*() const noexcept{
				return *get();
			}

			T* operator->() const noexcept{
				return get();
			}

			T& operator[](const std::size_t& index) const noexcept{
				return get()[index];
			}
			
			std::size_t useCount() const noexcept{
				return _count != nullptr ? *_count : 0;
			}

			bool unique() const noexcept{
				return useCount() == 1;
			}

			operator bool() const noexcept{
				return assetAlive() ? *_refPointer != nullptr : false;
			}

			template<typename... Args>
			static GuardedPtr<T> make(Args&&... args){
				T** memoryPointer = new T*(nullptr);
				*memoryPointer = new T(args...);

				GuardedPtr<T> ptr;
				ptr._count = new GuardedPtr<T>::CountType(1);
				ptr._refPointer = memoryPointer;

				return ptr;
			}

		private:
			using CountType = std::size_t;

			std::size_t increment() noexcept{
				return _count != nullptr ? (*_count)++ : 1;
			}

			std::size_t decrement() noexcept{
				return _count != nullptr ? (*_count)-- : 1;
			}

			bool assetAlive() const noexcept{
				return *_refPointer != nullptr;
			}

			std::size_t* _count;
			T** _refPointer;	
	};

	template<typename T, typename... Args>
	GuardedPtr<T> makeGuarded(Args&&... args){
		return GuardedPtr<T>::make(args...);
	}
}

#endif