#include <Raindrop/Graphics/Shaders/Shader.hpp>
#include <Raindrop/Graphics/Shaders/Context.hpp>

namespace Raindrop::Graphics::Shaders{
	Shader::Shader(Context& context, const std::vector<char>& code, VkShaderStageFlagBits stage) :
		_context{context},
		_stage{stage},
		_shader{VK_NULL_HANDLE}{

		auto& device = _context.graphics().internal().device();
		auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		VkShaderModuleCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.codeSize = code.size();
		info.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(device.get(), &info, allocationCallbacks, &_shader) != VK_SUCCESS){
			_context.logger().error("Failed to create a vulkan shader module");
			throw std::runtime_error("Failed to create shader module");
		}
	}

	Shader::~Shader(){
		auto& device = _context.graphics().internal().device();
		auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		if (_shader != VK_NULL_HANDLE){
			vkDestroyShaderModule(device.get(), _shader, allocationCallbacks);
			_shader = VK_NULL_HANDLE;
		}
	}

	VkShaderModule Shader::get() const{
		return _shader;
	}

	VkShaderStageFlagBits Shader::stage() const{
		return _stage;
	}
}