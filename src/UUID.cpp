#include <Raindrop/UUID.hpp>
#include <random>
#include <thread>

namespace Raindrop{
	UUID::UUID() noexcept {}

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

	UUID::UUID(UUID&& other) noexcept{
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
		return std::to_string(_lower) + std::to_string(_upper);
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
}