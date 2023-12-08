#ifndef __RAINDROP_GRAPHICS_FORMATS_LOADER_HPP__
#define __RAINDROP_GRAPHICS_FORMATS_LOADER_HPP__

#include <Raindrop/Graphics/Formats/common.hpp>

namespace Raindrop::Graphics::Formats{
	class Loader{
		public:
			Loader(Context& context);
			~Loader();

			void loadFormats(const YAML::Node& node);

		private:
			Context& _context;

			struct ImageRequirements{
				VkImageFormatProperties properties;
				VkImageType type;
				VkImageTiling tiling;
				VkImageUsageFlags usage;
				VkImageCreateFlags flags;
			};

			void loadFormat(const YAML::Node& node);
			void getCandidates(const YAML::Node& node, std::list<VkFormat>& candidates);
			void getFeatures(const YAML::Node& node, VkFormatProperties& features);
			void getImageRequirements(const YAML::Node& node, ImageRequirements& requirements);
	};
}

#endif