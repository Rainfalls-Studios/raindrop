#ifndef __RAINDROP_UTILS_FLAGS_HPP__
#define __RAINDROP_UTILS_FLAGS_HPP__

#include "common.hpp"
#include <iostream>

#define OPERATOR_COPY_HELPER(op) inline FlagsTemplate& operator op(const FlagsTemplate& other) noexcept {_bitset op other._bitset; return *this;} inline FlagsTemplate& operator op(const Bitset& bitset) noexcept {_bitset op bitset; return *this;} inline FlagsTemplate& operator op(const Bits& bit) noexcept {_bitset op bit; return *this;}
#define OPERATOR_CONST_HELPER(op) inline FlagsTemplate operator op(const FlagsTemplate& other) const noexcept {return _bitset op other._bitset;} inline FlagsTemplate operator op(const Bitset& bitset) const noexcept {return _bitset op bitset;} inline FlagsTemplate operator op(const Bits& bit) const noexcept {return _bitset op bit;}
#define OPERATOR_HELPER_PAIR(op) OPERATOR_COPY_HELPER(op##=); OPERATOR_CONST_HELPER(op);

#define OPERATOR_COMPAR_HELPER(op) inline bool operator op(const FlagsTemplate& other) const noexcept {return _bitset op other._bitset;} inline bool operator op(const Bitset& bitset) const noexcept {return _bitset op bitset;}

namespace Raindrop::Utils{
	template<typename Derived, typename Bitset_t = std::uint32_t>
	class FlagsTemplate {
		public:
			using Bitset = Bitset_t;
			enum Bits : Bitset;

			constexpr inline FlagsTemplate() noexcept : _bitset(0) {}
			constexpr inline FlagsTemplate(const Bitset& flags) noexcept : _bitset(flags) {}
			constexpr inline FlagsTemplate(const Bits& bit) noexcept : _bitset(bit) {}
			constexpr inline FlagsTemplate(const FlagsTemplate& other) noexcept : _bitset(other._bitset) {}


			inline FlagsTemplate(const std::initializer_list<Bits>& bits) noexcept {
				_bitset = 0;
				for (const auto& bit : bits) {
					_bitset |= static_cast<Bitset>(bit);
				}
			}

			template<typename... Args>
			constexpr FlagsTemplate(Args&&... args) {
				_bitset = 0;
				int i = 0;
				([&]
				{
					_bitset |= static_cast<Bitset>(args);
				} (), ...);
			}

			inline FlagsTemplate& operator=(const FlagsTemplate& other) noexcept{
				_bitset = other._bitset;
				return *this;
			}

			inline FlagsTemplate& operator=(const Bitset& bitset) noexcept{
				_bitset = bitset;
				return *this;
			}

			inline FlagsTemplate& operator=(const std::initializer_list<Bits>& bits) noexcept {
				_bitset = 0;
				for (const auto& bit : bits) {
					_bitset |= static_cast<Bitset>(bit);
				}
			}

			inline operator bool() const noexcept{
				return static_cast<bool>(_bitset);
			}

			inline operator const Bitset&() const noexcept{
				return _bitset;
			}

			OPERATOR_COMPAR_HELPER(==);
			OPERATOR_COMPAR_HELPER(!=);

			OPERATOR_HELPER_PAIR(|);
			OPERATOR_HELPER_PAIR(&);
			OPERATOR_HELPER_PAIR(^);

			inline constexpr const Bitset& get() const noexcept {
				return _bitset;
			}

			inline constexpr bool has(const Bits& bit) const noexcept{
				return _bitset & bit;
			}

			inline constexpr bool has(const Bitset& bitset) const noexcept{
				return (_bitset & bitset) == bitset;
			}

		private:
			Bitset _bitset;
	};
}

#undef OPERATOR_COMPAR_HELPER
#undef OPERATOR_HELPER_PAIR
#undef OPERATOR_CONST_HELPER
#undef OPERATOR_COPY_HELPER

#endif