#ifndef __RAINDROP_GRAPHICS_SHADERS_SHADER_COMPILER_HPP__
#define __RAINDROP_GRAPHICS_SHADERS_SHADER_COMPILER_HPP__

#include <Raindrop/Graphics/shaders/common.hpp>

namespace Raindrop::Graphics::Shaders{
	struct __ShaderCompileImpl;
	class ShaderCompiler{
		public:
			ShaderCompiler(GraphicsContext& context);
			~ShaderCompiler();

			std::vector<uint32_t> compile(const std::string& source, const char* name = "shader");
			void compile(const std::filesystem::path& source, const std::filesystem::path& destination);

		private:
			GraphicsContext& _context;
			__ShaderCompileImpl* _impl;

	};
}

#endif