#ifndef __RAINDROP_INTERNAL_TEMPLATES_RESOURCE_REGISTRY_HPP__
#define __RAINDROP_INTERNAL_TEMPLATES_RESOURCE_REGISTRY_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Templates{
	template<typename T, typename Handle_t = std::size_t>
	class ResourceRegistry{
		public:
			using Handle = Handle_t;

			static constexpr Handle INVALID_HANDLE = Handle(~0ULL);

			/**
			 * @brief The registry constructor
			 * @note By default, the registry will reserve 'DEFAULT_SIZE' elements
			 * 
			 */
			ResourceRegistry();
			~ResourceRegistry();

			/**
			 * @brief Create a new handles and assigns it an Handle
			 * 
			 * @param instance the instance to store
			 * @return Handle the assigned Handle
			 */
			Handle insert(const std::shared_ptr<T>& instance);

			/**
			 * @brief Removes an handle
			 * 
			 * @param Handle The Handle assigned to the handle to remove
			 */
			void remove(const Handle& Handle);

			/**
			 * @brief Checks if a handle exixts and is registred
			 * 
			 * @param Handle The Handle assigned to the handle to check
			 * @return true if the 
			 */
			bool exists(const Handle& Handle) const noexcept;

			/**
			 * @brief Get the handle assigned to the Handle
			 * 
			 * @param Handle The Handle supposedly assigned to a handle
			 * @return std::shared_ptr<T> 
			 */
			std::shared_ptr<T> get(const Handle& Handle) const;

			/**
			 * @brief Increases the capacity by 'n' elements
			 * 
			 * @param n The number of elements to increase by
			 */
			void reserve(const std::size_t& n);

			/**
			 * @brief Get the count of suposedly valHandle handles currently stored 
			 * 
			 * @return std::size_t 
			 */
			inline std::size_t getSize() const noexcept;

			/**
			 * @brief Get the maximal amount of handles teh registry can store without requirering a reallocation
			 * 
			 * @return std::size_t 
			 */
			inline std::size_t getCapacity() const noexcept;

			/**
			 * @brief Checks if the registry does not currently store any handle
			 * 
			 * @return true if the registry stores nothing
			 */
			inline bool isEmpty() const noexcept;

			/**
			 * @brief Releases all the non-used Handles that have not been properly signaled
			 * 
			 */
			inline void collectGarbage() const noexcept;

		private:
			std::vector<std::weak_ptr<T>> _handles;
			std::deque<Handle> _freeHandles;

			static constexpr std::size_t DEFAULT_SIZE = 10;
	};
}

#include "Inline/ResourceRegistry.inl"

#endif