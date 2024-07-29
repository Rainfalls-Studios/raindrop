#include <Raindrop/Graphics/ShaderModule.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics{
	ShaderModule::ShaderModule() noexcept :
		_context{nullptr},
		_module{VK_NULL_HANDLE},
		_info{}
	{}

	ShaderModule::~ShaderModule(){
		release();
	}

	void ShaderModule::prepare(Context& context){
		_context = &context;
		_info = std::make_unique<BuildInfo>();
	}

	void ShaderModule::initialize(){
		if (!_context){
			spdlog::warn("Attempt to initialized a non prepared shader module");
			throw std::runtime_error("The shader modyule has not been prepared !");
		}

		if (!_info){
			_context->logger->warn("Attempt to initialized a non prepared shader module");
			throw std::runtime_error("The shader modyule has not been prepared !");
		}

		const auto& code = _info->code;

		VkShaderModuleCreateInfo info{
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.pNext = nullptr,
			.flags = _info->flags,
			.codeSize = static_cast<uint32_t>(code.size()),
			.pCode = reinterpret_cast<const uint32_t*>(code.data())
		};

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		Exceptions::checkVkCreation<VkShaderModule>(
			vkCreateShaderModule(device.get(), &info, allocationCallbacks, &_module),
			"Failed to create shader module",
			_context->logger
		);
	}

	void ShaderModule::release(){
		if (!_context) return;

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		if (_module){
			_context->logger->trace("Destroying shader module...");

			vkDestroyShaderModule(device.get(), _module, allocationCallbacks);
			_module = VK_NULL_HANDLE;
		}

		_context = nullptr;
		_info.reset();
	}

	ShaderModule& ShaderModule::setCode(const std::string& code){
		getInfo().code = code;
		return *this;
	}

	ShaderModule& ShaderModule::setFlags(const VkShaderModuleCreateFlags& flags){
		getInfo().flags = flags;
		return *this;
	}

	const VkShaderModule& ShaderModule::get() const noexcept{
		return _module;
	}

	ShaderModule::BuildInfo& ShaderModule::getInfo(){
		if (!_context){
			spdlog::warn("Attempt to access change build info of a non prepared shader module");
			throw std::runtime_error("The shader module has not been prepared !");
		}

		if (!_info){
			_context->logger->warn("Attempt to access change build info of a non prepared shader module");
			throw std::runtime_error("The shader module has not been prepared !");
		}

		return *_info;
	}

}