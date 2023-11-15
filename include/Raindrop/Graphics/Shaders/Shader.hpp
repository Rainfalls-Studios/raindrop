#ifndef __RAINDROP_GRAPHICS_SHADER_HPP__
#define __RAINDROP_GRAPHICS_SHADER_HPP__

#include <Raindrop/Graphics/Shaders/common.hpp>
#include <Raindrop/Core/Asset/Asset.hpp>

namespace Raindrop::Graphics::Shaders{
	class Shader : public Core::Asset::Asset{
		public:
			Shader(Context& context, const std::vector<char>& code, VkShaderStageFlagBits stage);
			virtual ~Shader() override;

			VkShaderModule get() const;
			VkShaderStageFlagBits stage() const;

		private:
			Context& _context;
			VkShaderModule _shader;
			VkShaderStageFlagBits _stage;
	};
}

#endif