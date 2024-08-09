#ifndef __RAINDROP_UTILS_ALIGNEMENT_HPP__
#define __RAINDROP_UTILS_ALIGNEMENT_HPP__

#include "pch.pch"

namespace Raindrop::Utils{
	template<typename T>
	inline constexpr T alignTo(T value, std::size_t alignment) {
		return (value + alignment - 1) & ~(alignment - 1);
	}

	inline constexpr std::size_t calculatePadding(std::size_t size, std::size_t alignment) {
		return (alignment - (size % alignment)) % alignment;
	}
}

#endif