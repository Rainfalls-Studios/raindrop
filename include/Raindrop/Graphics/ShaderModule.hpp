#ifndef __RAINDROP_GRAPHICS_SHADER_MODULE_HPP__
#define __RAINDROP_GRAPHICS_SHADER_MODULE_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Graphics{
	class ShaderModule{
		public:
			ShaderModule() noexcept;
			~ShaderModule();

			void prepare(Context& context);
			void initialize();
			void release();

			ShaderModule& setCode(const std::string& code);
			ShaderModule& setFlags(const VkShaderModuleCreateFlags& flags);

			const VkShaderModule& get() const noexcept;
	
		private:
			struct BuildInfo{
				std::string code;
				VkShaderModuleCreateFlags flags;
			};

			Context* _context;
			VkShaderModule _module;

			std::unique_ptr<BuildInfo> _info;
			BuildInfo& getInfo();
			
	};
}

#endif