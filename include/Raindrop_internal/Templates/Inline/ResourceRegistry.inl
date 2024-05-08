#ifndef __RAINDROP_INTERNAL_TEMPLATES_INLINE_RESOURCE_REGISTRY_INL__
#define __RAINDROP_INTERNAL_TEMPLATES_INLINE_RESOURCE_REGISTRY_INL__

// TODO: tests

#include "../ResourceRegistry.hpp"

namespace Raindrop::Internal::Templates{
	template<typename T, typename Handle_t>
	ResourceRegistry<T, Handle_t>::ResourceRegistry(){
		reserve(DEFAULT_SIZE);
	}

	template<typename T, typename Handle_t>
	inline ResourceRegistry<T, Handle_t>::~ResourceRegistry(){
		_handles.clear();
		_freeHandles.clear();
	}

	template<typename T, typename Handle_t>
	void ResourceRegistry<T, Handle_t>::reserve(const std::size_t& n){
		std::size_t baseSize = _handles.size();

		// reserve the storage
		_handles.resize(baseSize + n);

		// emplace the new free Handles
		for (std::size_t i=0; i<n; i++){
			_freeHandles.push_back(i + baseSize);
		}
	}

	template<typename T, typename Handle_t>
	Handle_t ResourceRegistry<T, Handle_t>::insert(const std::shared_ptr<T>& instance){

		// If the free Handle pool is empty, simply reserve more
		if (_freeHandles.empty()){
			// doubles capacity
			reserve(_handles.size());
		}

		// query the first Handle available
		Handle handle = _freeHandles.front();
		_freeHandles.pop_front();

		// assignes it and return the Handle
		_handles.at(handle) = instance;
		return handle;
	}

	template<typename T, typename Handle_t>
	void ResourceRegistry<T, Handle_t>::remove(const Handle& handle){
		// removes the handle, this also cheks if the Handle is out of range
		_handles.at(handle).reset();

		// since the new Handle is supposedly free, push it
		_freeHandles.push_back(handle);
	}

	template<typename T, typename Handle_t>
	bool ResourceRegistry<T, Handle_t>::exists(const Handle& handle) const noexcept{
		if (handle >= _handles.size()) return false;

		// we already checked if the Handle is out of range, no needs to use 'at'
		return !_handles[handle].expired();
	}

	template<typename T, typename Handle_t>
	std::shared_ptr<T> ResourceRegistry<T, Handle_t>::get(const Handle& handle) const{
		// query the handle, this also checks that the Handle is in range
		auto& t = _handles.at(handle);

		// if the handle is expired, we can release the Handle
		if (t.expired()){
			_freeHandles.push_back(handle);
			return nullptr;
		}

		// if not, simply return the handle
		return t.lock();
	}

	template<typename T, typename Handle_t>
	inline std::size_t ResourceRegistry<T, Handle_t>::getSize() const noexcept{
		return _handles.size() - _freeHandles.size();
	}

	template<typename T, typename Handle_t>
	inline std::size_t ResourceRegistry<T, Handle_t>::getCapacity() const noexcept{
		return _handles.size();
	}

	template<typename T, typename Handle_t>
	inline bool ResourceRegistry<T, Handle_t>::isEmpty() const noexcept{
		return _handles.empty();
	}

	template<typename T, typename Handle_t>
	inline void ResourceRegistry<T, Handle_t>::collectGarbage() const noexcept{
		
		/**
		 * Optimized version using bitset and bitwise operators
		 * 
		 * Complexity : O(2n+m) 
		 * - n the number of handles
		 * - m the number of free Handles
		 * 
		 * Note that the complexity here denote the total number of iteration, and from that point of view,
		 * better than the 'naiv' way of iterating through every Handles for each and every free handles to check if they are already flagged 'free'.
		 * Where this strategy have a complexity of O(nm) (worst case scenario)
		 * 
		 */


		// initialy, we flag every expired handles in a bitset (1 for expired, otherwise 0)
		const std::size_t size = _handles.size();
		std::vector<char> bitset(size / 8);

		for (std::size_t i=0; i<size; i++){
			const std::size_t bit = i % 8;
			const std::size_t byte = i / 8;

			const bool expired = _handles[i].expired();

			bitset[byte] |= expired << bit;

			/**
			 * Example
			 * 
			 * original bitset
			 * 00100110
			 * 
			 * we are working on the fifth bit (index 4), thus
			 * expired << bit
			 * 1 << 4 -> 00010000
			 * 
			 * and then, thanks to the 'or' bitwise operator
			 * 00100110 |= 000100000 -> 00110110
			 */
		}

		// the, we flag only free Handles as 0, this was, only non-free yet expired Handles will be shown up

		for (const auto& Handle : _freeHandles){
			const std::size_t bit = Handle % 8;
			const std::size_t byte = Handle / 8;

			/**
			 * Example
			 * 
			 * original bitset
			 * 00110110
			 * 
			 * we are working on the fifth bit (still index 4) and we found out that this Handle is already stores in the free Handle pool
			 * 1 << bit
			 * 1 << 4 -> 00010000
			 * 
			 * we invert it
			 * ~(1 << 4) -> 11101111
			 * 
			 * and we, thank to the 'and' bitwize operator
			 * 00110110 & ~(1 << 4)
			 * 00110110 & 11101111 -> 00100110
			 */

			bitset[byte] &= ~(1 << bit);
		}

		// now, we can iterate through every bits and find the Handle we want to collect

		for (std::size_t i=0; i<size; i++){
			const std::size_t bit = i % 8;
			const std::size_t byte = i / 8;

			/**
			 * Here a simple bit shift and 'and'
			 * 
			 * original bitste
			 * 00100110
			 * 
			 * we shit it 4 bit to the right
			 * 00000010
			 * 
			 * and apply an 'and'
			 * 
			 * 00000010 & 00000001 -> 00000000
			 * 
			 * here, we can see that the current bit is 0, and so the Handle is either not expired or is already free
			 */
			
			if ((bitset[byte] >> bit) & 1){
				_freeHandles.push_back(i);
			}
		}
	}
}

#endif