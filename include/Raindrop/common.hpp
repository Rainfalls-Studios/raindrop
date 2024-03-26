#ifndef __RAINDROP_COMMON_HPP__
#define __RAINDROP_COMMON_HPP__

#include "pch.pch"

namespace Raindrop{
	struct Context;
	class Raindrop;
	
	using Path = std::filesystem::path;

	struct Version{
		std::size_t variant;
		std::size_t major;
		std::size_t minor;
		std::size_t patch;
	};

	Version makeVersion(std::size_t variant, std::size_t major, std::size_t minor, std::size_t patch);
}

#endif