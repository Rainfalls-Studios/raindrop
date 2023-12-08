#ifndef __RAINDROP_GRAPHICS_SHADERS_GRAPHICS_PIPELINE_LOADER_YAML_LOADER_HPP__
#define __RAINDROP_GRAPHICS_SHADERS_GRAPHICS_PIPELINE_LOADER_YAML_LOADER_HPP__

#include <Raindrop/Graphics/Shaders/GraphicsPipelineLoaders/common.hpp>
#include <Raindrop/Graphics/Shaders/GraphicsPipelineLoaders/Loader.hpp>

#include <yaml-cpp/yaml.h>

namespace Raindrop::Graphics::Shaders::GraphicsPipelineLoaders{
	class YamlLoader : public Loader{
		public:
			YamlLoader(Context& context, const std::filesystem::path& path);
			virtual ~YamlLoader() override = default;

		private:
			void loadShaders(const YAML::Node& node);
			void loadTarget(const YAML::Node& node);
			void loadRasterizationInfo(const YAML::Node& node);
			void loadMultisampleInfo(const YAML::Node& node);
			void loadDepthStencilInfo(const YAML::Node& node);

			VkStencilOpState loadStencilOpState(const YAML::Node& node);
	};
}

#endif