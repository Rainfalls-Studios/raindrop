#include <Raindrop/Graphics/Core/Instance.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <set>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>


static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData){
	Raindrop::Graphics::Core::Context* context = static_cast<Raindrop::Graphics::Core::Context*>(pUserData);

	spdlog::level::level_enum level = spdlog::level::info;

	switch (messageSeverity){
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: level = spdlog::level::trace; break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: level = spdlog::level::info; break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: level = spdlog::level::warn; break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: level = spdlog::level::err; break;
	}

	context->logger->log(level, "VULKAN {} :: {}", pCallbackData->pMessageIdName, pCallbackData->pMessage);
	return VK_FALSE;
}

static inline VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger){
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func) return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	return VK_ERROR_EXTENSION_NOT_PRESENT;
}

static inline void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator){
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func) func(instance, debugMessenger, pAllocator);
}

namespace Raindrop::Graphics::Core{
	Instance::Instance() noexcept :
		_context{nullptr},
		_instance{},
		_builder{}
	{}
	
	Instance::~Instance(){
		release();
	}

	void Instance::prepare(Context& context){
		_builder = std::make_unique<vkb::InstanceBuilder>();
		_instance = {};
		_context = &context;
	}
	
	void Instance::initialize(){
		_builder->set_debug_callback(debugCallback);
		_builder->set_debug_callback_user_data_pointer(_context);
		_builder->set_debug_messenger_severity(
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);

		_context->logger->info("Creating vulkan instance...");

		auto result = _builder->build();

		if (!result){
			_context->logger->error("Failed to create vulkan instance : {}", result.error().message());
			throw std::runtime_error("Failed to create vulkan instance");
		}

		_instance = result.value();
		_builder.reset();
	}

	void Instance::release(){
		_context->logger->info("Destroying vulkan instance...");

		_builder.reset();
		vkb::destroy_instance(_instance);
		_context = nullptr;
	}
	
	vkb::InstanceBuilder* Instance::operator->() noexcept{
		return _builder.get();
	}

	vkb::InstanceBuilder& Instance::operator*() noexcept{
		return *_builder.get();
	}
	
	const VkInstance& Instance::get() const noexcept{
		return _instance.instance;
	}

	const vkb::Instance& Instance::getVkb() const noexcept{
		return _instance;
	}

}