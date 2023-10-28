#ifndef __RAINDROP_GRAPHICS_UTILS_STRING_TO_VULKAN_HPP__
#define __RAINDROP_GRAPHICS_UTILS_STRING_TO_VULKAN_HPP__

#include <Raindrop/Graphics/Utils/common.hpp>

namespace Raindrop::Graphics::Utils{
	class StringToVulkan{
		public:
			StringToVulkan(Context& context);
			~StringToVulkan();

			uint32_t toEnum(const std::string& str, uint32_t d);
			template<typename T> T toEnum(const std::string& str, T d = {});

		private:
			Context& _context;
			static const std::unordered_map<std::string, uint32_t> _map;
	};

	#include <Raindrop/Graphics/Utils/StringToVulkan.hpp>
}

#endif