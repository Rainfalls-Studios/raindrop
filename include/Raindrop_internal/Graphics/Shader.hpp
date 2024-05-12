#ifndef __RAINDROP_INTERTNAL_GRAPHICS_SAMPLER_HPP__
#define __RAINDROP_INTERTNAL_GRAPHICS_SAMPLER_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics{
	struct ShaderConfigInfo{
		std::vector<char> code;
	};

	class Shader{
		public:
			Shader(Context& context, const ShaderConfigInfo& info);
			~Shader();

			VkShaderModule get() const;

		private:
			Context& _context;
			VkShaderModule _module;
	};
}

#endif