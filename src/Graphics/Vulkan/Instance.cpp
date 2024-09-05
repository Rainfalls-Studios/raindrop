#include "Raindrop/Graphics/Vulkan/Instance.hpp"
#include "Raindrop/Graphics/Context.hpp"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData){
	Raindrop::Graphics::Context* context = static_cast<Raindrop::Graphics::Context*>(pUserData);

	spdlog::level::level_enum level = spdlog::level::info;

	switch (messageSeverity){
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: level = spdlog::level::trace; break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: level = spdlog::level::info; break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: level = spdlog::level::warn; break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: level = spdlog::level::err; break;
	}

	SPDLOG_LOGGER_CALL(context->getLogger(), level, "VULKAN {} :: {}", pCallbackData->pMessageIdName, pCallbackData->pMessage);
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

namespace Raindrop::Graphics::Vulkan{
	Instance::Instance(Context& context, const std::vector<const char*> extensions) :
		_context{context},
		_instance{}
	{
		SPDLOG_LOGGER_INFO(_context.getLogger(), "Creating the vulkan instance...");
		vkb::InstanceBuilder builder;
		
		#ifndef NDEBUG
			builder.request_validation_layers();
		#endif

		builder
			.enable_extensions(extensions)
			.set_engine_name("Raindrop")
			.set_debug_callback(debugCallback)
			.set_debug_callback_user_data_pointer(&_context)
			.set_debug_messenger_severity(
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
			)
			.set_debug_messenger_type(
				VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
			);
		
		vkb::Result result = builder.build();
		if (!result){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to create the vulkan instance : {}", result.error().message());
			throw std::runtime_error("Failed to create the device");
		}

		SPDLOG_LOGGER_INFO(_context.getLogger(), "Vulkan instance created successfully");
		_instance = *result;
	}
	
	Instance::~Instance(){
		vkb::destroy_instance(_instance);
	}
	
	const VkInstance& Instance::get() const noexcept{
		return _instance.instance;
	}

	const vkb::Instance& Instance::getVkb() const noexcept{
		return _instance;
	}
}