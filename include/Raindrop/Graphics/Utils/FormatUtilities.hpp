#ifndef __RAINDROP_GRAPHICS_UTILS_FORMAT_UTILITIES_HPP__
#define __RAINDROP_GRAPHICS_UTILS_FORMAT_UTILITIES_HPP__

#include <Raindrop/Graphics/Utils/common.hpp>

namespace Raindrop::Graphics::Utils{
	class FormatUtilities{
		public:
			FormatUtilities(Context& context);

			VkFormat bestFormatBuffer(uint32_t flags, VkFormatFeatureFlags features);
			VkFormat bestFormatOptimalTiling(uint32_t flags, VkFormatFeatureFlags features);
			VkFormat bestFormatLinearTiling(uint32_t flags, VkFormatFeatureFlags features);

			bool is(VkFormat format, uint32_t flags);
			std::size_t size(VkFormat format);

		private:
			Context& _context;

			std::set<VkFormat> getSet(uint32_t flags);
	};
}

#endif