#ifndef __RAINDROP_INTERNAL_REGISTRY_HPP__
#define __RAINDROP_INTERNAL_REGISTRY_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Templates{
	template<typename T, typename ID_t = std::size_t>
	class Registry{
		public:
			using ID = ID_t;
			static constexpr ID INVALID_ID = ID(~0ULL);

			Registry();
			~Registry();

			ID insert(const T& t);
			void remove(const ID& id);
			void reserve(const std::size_t& n);

			T& get(const ID& id);
			const T& get(const ID& id) const;

			inline std::size_t getSize() const noexcept;
			inline std::size_t getCapacity() const noexcept;
			inline bool isEmpty() const noexcept;

		
		private:
			std::vector<T> _handles;
			std::deque<ID> _freeIDs;

			static constexpr std::size_t DEFAULT_SIZE = 10;
	};
}

#include "Inline/Registry.inl"

#endif