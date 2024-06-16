#ifndef __RAINDROP_UTILS_FLAGS_HPP__
#define __RAINDROP_UTILS_FLAGS_HPP__

#include "common.hpp"

namespace Raindrop::Utils{
	template <typename T>
	class FlagsTemplate{
		public:
			using Bits = T;
			using UnderlyingType = typename std::underlying_type<T>::type;

			constexpr inline FlagsTemplate() noexcept : _flags{0} {}
			constexpr inline FlagsTemplate(T flag) noexcept : _flags{static_cast<UnderlyingType>(flag)} {}
			constexpr inline FlagsTemplate(UnderlyingType flags) noexcept : _flags{flags} {}

			constexpr inline FlagsTemplate(const std::initializer_list<T>& flags) : _flags{0}{
				for (const auto& f : flags){
					_flags |= static_cast<UnderlyingType>(f);
				}
			}

			constexpr inline FlagsTemplate& operator|=(T flag) noexcept{
				_flags |= static_cast<UnderlyingType>(flag);
				return *this;
			}

			constexpr inline FlagsTemplate& operator&=(T flag) noexcept{
				_flags &= static_cast<UnderlyingType>(flag);
				return *this;
			}

			constexpr inline FlagsTemplate operator|(T flag) const noexcept{
				return FlagsTemplate(_flags | static_cast<UnderlyingType>(flag));
			}

			constexpr inline FlagsTemplate operator&(T flag) const noexcept{
				return FlagsTemplate(_flags & static_cast<UnderlyingType>(flag));
			}

			constexpr inline bool operator==(T flag) const noexcept{
				return _flags == static_cast<UnderlyingType>(flag);
			}

			constexpr inline explicit operator bool() const noexcept{
				return _flags != 0;
			}

			constexpr inline void clear() noexcept {
				_flags = 0;
			}

			/**
			 * Sets the flags to true
			 */
			constexpr inline void set(T flag) noexcept{
				_flags |= static_cast<UnderlyingType>(flag);
			}

			/**
			 * Sets the flags to stat's value
			 */
			constexpr inline void set(T flag, const bool& state) noexcept{
				if (state){
					set(flag);
				} else {
					unset(flag);
				}
			}

			/**
			 * Sets the flags to false
			 */
			constexpr inline void unset(T flag) noexcept {
				_flags &= ~static_cast<UnderlyingType>(flag);
			}

			/**
			 * Checks if the flags is set to true
			 */
			constexpr inline bool has(T flag) const noexcept {
				return (_flags & static_cast<UnderlyingType>(flag)) != 0;
			}
			
			/**
			 * Checks if all flags in 'other' are set in this object's flags
			 */
			constexpr inline bool has(const FlagsTemplate<T>& other) const noexcept {
				return (_flags & other._flags) == other._flags;
			}
			
			/**
			 * Checks if at least one flags in 'other' is set in this object's flags
			 */
			constexpr inline bool overlap(const FlagsTemplate<T>& other) const noexcept {
				return (_flags & other._flags) != 0;
			}

			/**
			 * Checks if no flags set in 'other' are set in this object's flags
			 */
			constexpr inline bool disjoint(const FlagsTemplate<T>& other) const noexcept {
				return (_flags & other._flags) == 0;
			}

			constexpr inline const UnderlyingType& get() const noexcept{
				return _flags;
			}

		private:
			UnderlyingType _flags;
	};
}

#define RAINDROP_FLAGS_CLASS(name, bits) \
	class name : public ::Raindrop::Utils::FlagsTemplate<bits>{ \
		public: \
			using ::Raindrop::Utils::FlagsTemplate<bits>::FlagsTemplate; \
			using enum bits; \
	};

#endif