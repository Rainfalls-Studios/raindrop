#include <Raindrop_internal/Graphics/Shader.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics{
	Shader::Shader(Context& context, const ShaderConfigInfo& info) : 
		_context{context},
		_module{VK_NULL_HANDLE}
	{
		
		auto& device = context.getDevice();
		auto& allocationCallbacks = context.getAllocationCallbacks();

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = info.code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(info.code.data());

		Exceptions::checkVkCreation<VkShaderModule>(
			vkCreateShaderModule(device.get(), &createInfo, allocationCallbacks, &_module),
			"Failed to create shader module",
			_context.getLogger()
		);
	}

	Shader::~Shader(){
		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

		if (_module != VK_NULL_HANDLE){
			vkDestroyShaderModule(device.get(), _module, allocationCallbacks);
			_module = VK_NULL_HANDLE;
		}
	}

	VkShaderModule Shader::get() const{
		return _module;
	}
}