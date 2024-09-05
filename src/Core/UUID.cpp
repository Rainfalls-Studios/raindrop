#include "Raindrop/Core/UUID.hpp"

namespace Raindrop::Core{
	UUID::UUID() noexcept :
		_lower{0},
		_upper{0}
	{}

	UUID::UUID(const std::uint64_t& lower, const std::uint64_t& upper) noexcept :
		_lower{lower},
		_upper{upper}
	{}

	UUID::UUID(const UUID& other) noexcept : 
		_lower{other._lower},
		_upper{other._upper}
	{}

	UUID& UUID::operator=(const UUID& other) noexcept{
		_lower = other._lower;
		_upper = other._upper;
		return *this;
	}

	UUID::UUID(UUID&& other) noexcept :
		_lower{0},
		_upper{0}
	{
		swap(*this, other);
	}

	UUID& UUID::operator=(UUID&& other) noexcept{
		swap(*this, other);
		return *this;
	}

	void swap(UUID& A, UUID& B) noexcept{
		std::swap(A._lower, B._lower);
		std::swap(A._upper, B._upper);
	}

	bool UUID::operator==(const UUID& other) const noexcept{
		return _lower == other._lower && _upper == other._upper;
	}

	std::string UUID::asString() const{
		char buffer[33]; // 16 digits for each uint64_t + 1 for null terminator
		std::sprintf(buffer, "%016" PRIu64 "%016" PRIu64, _lower, _upper);
		return std::string(buffer);
	}

	bool UUID::parse(const std::string& str) noexcept{
		if (str.size() < 16 * 2) {
			return false;
		}

		const char* cstr = str.c_str();

		char* end = nullptr;
		const std::uint64_t temp_lower = std::strtoull(cstr, &end, 10);

		if (end - cstr != 16) {
			return false;
		}

		const std::uint64_t temp_upper = std::strtoull(end, nullptr, 10);

		if (end && *end == '\0'){
			_lower = temp_lower;
			_upper = temp_upper;
			return true;
		}

		return false;
	}


	bool UUID::operator<(const UUID& other) const noexcept{
		if (_upper < other._upper){
			return true;
		} else if (_upper > other._lower) {
			return false;
		}

		return _lower < other._lower;
	}

	bool UUID::operator>(const UUID& other) const noexcept{
		return !(*this < other);
	}

	UUID UUID::Generate() noexcept{
		thread_local static std::mt19937_64 rng(std::random_device{}());
		std::uniform_int_distribution<std::uint64_t> dist;

		// Generate two random uint64_t values
		const std::uint64_t lower = dist(rng);
		const std::uint64_t upper = dist(rng);

		return UUID(lower, upper);
	}


	const std::uint64_t& UUID::lower() const noexcept{
		return _lower;
	}

	const std::uint64_t& UUID::upper() const noexcept{
		return _upper;
	}
}