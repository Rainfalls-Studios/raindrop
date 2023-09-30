#ifndef __RAINDROP_CORE_CONFIGURATION_HPP__
#define __RAINDROP_CORE_CONFIGURATION_HPP__

#include <Raindrop/Core/common.hpp>

namespace Raindrop::Core{
	struct Configuration{
		Configuration(const std::filesystem::path& configFile);

		std::filesystem::path shaderDirectory;
		std::filesystem::path compiledShaderDirectory;
	};
}

#endif