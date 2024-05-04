#ifndef __RAINDROP_UTILS_FLAGS_HPP__
#define __RAINDROP_UTILS_FLAGS_HPP__

#include "common.hpp"

#define OPERATOR_COPY_HELPER(op) inline FlagsTemplate& operator op(const FlagsTemplate& other) noexcept {_biteset op other._biteset; return *this;} inline FlagsTemplate& operator op(const Bitset& bitset) noexcept {_biteset op bitset; return *this;}
#define OPERATOR_CONST_HELPER(op) inline FlagsTemplate operator op(const FlagsTemplate& other) const noexcept {return _biteset op other._biteset;} inline FlagsTemplate operator op(const Bitset& bitset) const noexcept {return _biteset op bitset;}
#define OPERATOR_HELPER_PAIR(op) OPERATOR_COPY_HELPER(op##=); OPERATOR_CONST_HELPER(op);

#define OPERATOR_COMPAR_HELPER(op) inline bool operator op(const FlagsTemplate& other) const noexcept {return _biteset op other._biteset;} inline bool operator op(const Bitset& bitset) const noexcept {return _biteset op bitset;}

namespace Raindrop::Utils{
	template<typename Derived, typename Bitset_t = std::uint32_t>
	class FlagsTemplate {
		public:
			using Bitset = Bitset_t;
			enum Bits : Bitset;

			inline FlagsTemplate() noexcept : _biteset(0) {}
			inline FlagsTemplate(const Bitset& flags) noexcept : _biteset(flags) {}
			inline FlagsTemplate(const FlagsTemplate& other) noexcept : _biteset(other._biteset) {}

			inline FlagsTemplate(const std::initializer_list<Bits>& bits) noexcept {
				for (const auto& bit : bits) {
					_biteset |= static_cast<Bitset>(bit);
				}
			}

			inline FlagsTemplate& operator=(const FlagsTemplate& other) noexcept{
				_biteset = other._biteset;
				return *this;
			}

			inline FlagsTemplate& operator=(const Bitset& bitset) noexcept{
				_biteset = bitset;
				return *this;
			}

			inline operator bool() const noexcept{
				return static_cast<bool>(_biteset);
			}

			inline operator const Bitset&() const noexcept{
				return _biteset;
			}

			OPERATOR_COMPAR_HELPER(==);
			OPERATOR_COMPAR_HELPER(!=);

			OPERATOR_HELPER_PAIR(|);
			OPERATOR_HELPER_PAIR(&);
			OPERATOR_HELPER_PAIR(^);

			inline const Bitset& get() const noexcept {
				return _biteset;
			}

			inline bool has(const Bits& bit) const noexcept{
				return _biteset & bit;
			}

		private:
			Bitset _biteset;
	};
}

#undef OPERATOR_COMPAR_HELPER
#undef OPERATOR_HELPER_PAIR
#undef OPERATOR_CONST_HELPER
#undef OPERATOR_COPY_HELPER

#endif