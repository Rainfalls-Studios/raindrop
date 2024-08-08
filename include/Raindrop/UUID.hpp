#ifndef __RAINDROP_UUID_HPP__
#define __RAINDROP_UUID_HPP__

#include "pch.pch"

namespace Raindrop{
	class UUID{
		public:
			UUID() noexcept;
			~UUID() = default;

			UUID(const std::uint64_t& lower, const std::uint64_t& upper) noexcept;

			UUID(const UUID& other) noexcept;
			UUID& operator=(const UUID& other) noexcept;

			UUID(UUID&& other) noexcept;
			UUID& operator=(UUID&& other) noexcept;

			/**
			 * @brief Generate a new random UUID
			 * 
			 * The UUID is randomly generated and does not check for previous collisions,
			 * however, the UUID beeing represented as two uint64_ts, the probability of a collisions would be about 1 over 2 power of 128, which is highly improbable
			 * 
			 * @return UUID 
			 */
			static UUID Generate() noexcept;


			std::string asString() const;

			friend void swap(UUID& A, UUID& B) noexcept;

			bool operator==(const UUID& other) const noexcept;
			bool operator<(const UUID& other) const noexcept;
			bool operator>(const UUID& other) const noexcept;

		private:
			std::uint64_t _lower;
			std::uint64_t _upper;
	};
}

#endif