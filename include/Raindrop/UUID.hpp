#ifndef __RAINDROP_UUID_HPP__
#define __RAINDROP_UUID_HPP__

#include "pch.pch"

namespace Raindrop{
	class UUID{
		public:
			UUID() noexcept;
			~UUID() = default;

			/**
			 * @brief Construct a new UUID
			 * 
			 * @param lower lower uuid index
			 * @param upper upper uuid index
			 */
			UUID(const std::uint64_t& lower, const std::uint64_t& upper) noexcept;

			/**
			 * @brief Copy a new UUID form another one  
			 * 
			 * @param other the UUID to copy
			 */
			UUID(const UUID& other) noexcept;

			/**
			 * @brief Copy an UUID
			 * 
			 * @param other The UUID to copy
			 * @return UUID& 
			 */
			UUID& operator=(const UUID& other) noexcept;

			/**
			 * @brief Swap other to a new UUID
			 * 
			 * @param other the UUID to move
			 */
			UUID(UUID&& other) noexcept;

			/**
			 * @brief Move uuid
			 * 
			 * @param other the uuid to move
			 * @return UUID& 
			 */
			UUID& operator=(UUID&& other) noexcept;

			/**
			 * @brief Swap two UUID with one another
			 * 
			 * @param A UUID A
			 * @param B UUID B
			 */
			friend void swap(UUID& A, UUID& B) noexcept;

			/**
			 * @brief Generate a new random UUID
			 * 
			 * The UUID is randomly generated and does not check for previous collisions,
			 * however, the UUID beeing represented as two uint64_ts, the probability of a collisions would be about 1 over 2 power of 128, which is highly improbable
			 * 
			 * @return UUID 
			 */
			static UUID Generate() noexcept;

			/**
			 * @brief Construct a string from the UUID content
			 * 
			 * @return std::string 
			 */
			std::string asString() const;

			/**
			 * @brief parse a UUID from a string
			 * 
			 * @param string the string to pass
			 * @return true on success, false otherwise
			 */
			bool parse(const std::string& string) noexcept;

			/**
			 * @brief Checks if two UUIDs are the same
			 * 
			 * @param other the uuid to check with
			 * @return true if the the two UUID are the same, false otherwisr
			 */
			bool operator==(const UUID& other) const noexcept;

			/**
			 * @brief Checks if an UUID is greater than another one. Used for faster access in data structure
			 * 
			 * @param other The other UUID to check with
			 * @return true if 'other' is greater than the current instance
			 */
			bool operator<(const UUID& other) const noexcept;

			/**
			 * @brief Checks if an UUID is smaller than another one. Used for faster access in data structure
			 * 
			 * @param other The other UUID to check with
			 * @return true if 'other' is smaller than the current instance
			 */
			bool operator>(const UUID& other) const noexcept;

			/**
			 * @brief A direct acess to the UUID lower chuck
			 * 
			 * @return const std::uint64_t& 
			 */
			const std::uint64_t& lower() const noexcept;

			/**
			 * @brief A direct acess to the UUID upper chunk
			 * 
			 * @return const std::uint64_t& 
			 */
			const std::uint64_t& upper() const noexcept;

		private:
			std::uint64_t _lower;
			std::uint64_t _upper;
	};
}

namespace std{
	template<>
	struct hash<Raindrop::UUID>{
		std::size_t operator()(const Raindrop::UUID& k) const{
			const std::size_t hash = static_cast<std::size_t>(k.lower());
			return hash ^ (k.upper() + 0x9e3779b9 + (hash << 6) + (hash >> 2));
		}
	};
}

#endif