#ifndef __RAINDROP_CORE_MEMORY_GUARDED_POINTER_HPP__
#define __RAINDROP_CORE_MEMORY_GUARDED_POINTER_HPP__

#include "common.hpp"

namespace Raindrop::Core::Memory{
	template<typename T>
	class GuardedPtr{
		public:
			GuardedPtr() : _count{nullptr}, _ref{nullptr}{}
			GuardedPtr(const GuardedPtr& other) : _count{other._count}, _ref{ref}{
				increment();
			}

			~GuardedPtr(){
				reset();
			}

			GuardedPtr& operator=(const GuardedPtr& other){
				_count = other._count;
				_ref = other._ref;
				increment();
			}

			void reset(){
				if (decrement() == 0){
					delete _count;
					_count = nullptr;

					delete _ref;
					_ref = nullptr;
				}
			}

			T* get() const noexcept{
				return _ref;
			}

			T& operator*() const noexcept{
				return *_ref;
			}

			T* operator->() const noexcept{
				return _ref;
			}

			T& operator[](const std::size_t& index) const noexcept{
				return _ref[index];
			}
			
			std::size_t useCount() const noexcept{
				return _count != nullptr ? *_count : 0;
			}

			bool unique() const noexcept(){
				return useCount() == 1;
			}

			operator bool() const noexcept(){
				return _ref != nullptr;
			}

		private:
			using CountType = std::size_t;

			template<typename... Args>
			friend Memory::makeGuarded<T>(Args&&... args);

			std::size_t increment() noexcept{
				return _count != nullptr ? (*_count)++ : 1;
			}

			std::size_t decrement() noexcept{
				return _count != nullptr ? (*_count)-- : 1;
			}


			std::size_t* _count;
			T* _ref;
			
	};

	template<typename T, typename... Args>
	GuardedPtr<T> makeGuarded(Args&&... args){
		T* memory = new T(args...);

		GuardedPtr<T> ptr;
		ptr._count = new CountType(1);
		ptr._ref = memory;

		return ptr;
	}
}

#endif