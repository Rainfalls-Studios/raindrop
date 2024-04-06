#ifndef __RAINDROP_UTILS_REGISTRY_HPP__
#define __RAINDROP_UTILS_REGISTRY_HPP__

#include "common.hpp"

namespace Raindrop::Utils{
	template<typename T, typename ID = std::size_t, typename... Default_T_Args>
	class Registry{
		protected:
			using Default_T = typename std::conditional<std::is_pointer_v<T>, std::nullptr_t, T>::type;

			Registry() = default;

			virtual ~Registry(){
				_freeIDs.clear();
			}

			ID registerHandle(T&& t){
				if (_freeIDs.empty()){
					_handles.push_back(std::move(t));
					return static_cast<ID>(_handles.size() - 1);
				}

				ID id = _freeIDs.front();
				_freeIDs.pop_front();

				_handles[id] = std::move(t);
				return id;
			}

			void destroy(const ID& id){
				if (inRange(id)){
					_handles[id] = Default_T();
					_freeIDs.push_back(id);
				}
			}

			T& get(const ID& id){
				assert(inRange(id));
				return _handles[id];
			}

			const T& get(const ID& id) const{
				assert(inRange(id));
				return _handles[id];
			}

			bool exists(const ID& id) const noexcept{
				auto it = std::find(_freeIDs.begin(), _freeIDs.end(), id);
				return it == _freeIDs.end();
			}

			bool inRange(const ID& id) const noexcept{
				return id < _handles.size();
			}

			std::size_t reservedSize() const noexcept{
				return _handles.size();
			}

			std::size_t freeSize() const noexcept{
				return _freeIDs.size();
			}

			std::size_t usedSize() const noexcept{
				return reservedSize() - freeSize();
			}

			std::vector<T> _handles;
			std::deque<ID> _freeIDs;
	};
}

#endif