#ifndef __RAINDROP_INTERNAL_TEMPLATES_INLINE_REGISTRY_INL__
#define __RAINDROP_INTERNAL_TEMPLATES_INLINE_REGISTRY_INL__

#include "../Registry.hpp"

namespace Raindrop::Internal::Templates{

	template<typename T, typename ID_t>
	Registry<T, ID_t>::Registry(){
		reserve(DEFAULT_SIZE);
	}

	template<typename T, typename ID_t>
	Registry<T, ID_t>::~Registry(){
		_handles.clear();
		_freeIDs.clear();
	}

	template<typename T, typename ID_t>
	ID_t Registry<T, ID_t>::insert(const T& t){
		if (_freeIDs.empty()){
			reserve(_handles.size());
		}

		ID id = _freeIDs.front();
		_freeIDs.pop_front();

		_handles.at(id) = t;
		return id;
	}

	template<typename T, typename ID_t>
	void Registry<T, ID_t>::remove(const ID& id){
		_freeIDs.push_back(id);
	}

	template<typename T, typename ID_t>
	void Registry<T, ID_t>::reserve(const std::size_t& n){
		std::size_t baseSize = _handles.size();

		_handles.resize(baseSize + n);

		for (std::size_t i=0; i<n; i++){
			_freeIDs.push_back(i + baseSize);
		}
	}

	template<typename T, typename ID_t>
	T& Registry<T, ID_t>::get(const ID& id){
		return _handles.at(id);
	}
	
	template<typename T, typename ID_t>
	const T& Registry<T, ID_t>::get(const ID& id) const{
		return _handles.at(id);
	}

	template<typename T, typename ID_t>
	inline std::size_t Registry<T, ID_t>::getSize() const noexcept{
		return _handles.size() - _freeIDs.size();
	}

	template<typename T, typename ID_t>
	inline std::size_t Registry<T, ID_t>::getCapacity() const noexcept{
		return _handles.size();
	}

	template<typename T, typename ID_t>
	inline bool Registry<T, ID_t>::isEmpty() const noexcept{
		return getSize() == 0;
	}
}

#endif