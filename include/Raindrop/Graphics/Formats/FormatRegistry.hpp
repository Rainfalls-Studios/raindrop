#ifndef __RAINDROP_GRAPHICS_FORMATS_FORMAT_REGISTRY_HPP__
#define __RAINDROP_GRAPHICS_FORMATS_FORMAT_REGISTRY_HPP__

#include <Raindrop/Graphics/Formats/common.hpp>

namespace Raindrop::Graphics::Formats{
	class FormatRegistry{
		public:
			FormatRegistry(Context& context);
			~FormatRegistry();

			void registerFormat(VkFormat format, const std::string& name);
			VkFormat get(const std::string& name) const;
		
		private:
			Context& _context;

			std::unordered_map<std::string, VkFormat> _formats;
	};
}

#endif