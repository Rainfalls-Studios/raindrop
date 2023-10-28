#include <Raindrop/Graphics/Internal/Instance.hpp>
#include <Raindrop/Graphics/Internal/Window.hpp>
#include <Raindrop/Graphics/Internal/Context.hpp>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData){
	Raindrop::Graphics::Internal::Context* context = static_cast<Raindrop::Graphics::Internal::Context*>(pUserData);
	spdlog::level::level_enum level;
	switch (messageSeverity){
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: level = spdlog::level::trace; return VK_FALSE;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: level = spdlog::level::info; return VK_FALSE;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: level = spdlog::level::warn; return VK_FALSE;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: level = spdlog::level::err; return VK_FALSE;
	}

	context->logger.log(level, "%s", pCallbackData->pMessage);
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

namespace Raindrop::Graphics::Internal{
	Instance::Instance(Context& context) : _context{context}{
		_context.logger.info("Initializing vulkan Instance...");
		_requiredExtensions.insert(_requiredExtensions.end(), REQUIRED_EXTENSIONS.begin(), REQUIRED_EXTENSIONS.end());
		requireGraphicsExtensions();
		build();

		_context.window.createSurface();
		_context.logger.info("Vulkan Instance initialized without any critical error");
	}

	Instance::~Instance(){
		_context.window.destroySurface();
		if (_instance){
			vkDestroyInstance(_instance, _context.graphics.allocationCallbacks);
			_instance = VK_NULL_HANDLE;
		}
	}
	
	VkInstance Instance::get() const{
		return _instance;
	}

	void Instance::build(){
		VkInstanceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		std::vector<const char*> layers;
		layers.reserve(REQUIRED_VALIDATION_LAYERS.size() + REQUIRED_LAYERS.size());

		if (REQUIRE_VALIDATION){
			checkValidationLayers();
			setupDebugMessenger(debugCreateInfo);
			info.pNext = &debugCreateInfo;

			layers.insert(layers.end(), REQUIRED_VALIDATION_LAYERS.begin(), REQUIRED_VALIDATION_LAYERS.end());
		}

		layers.insert(layers.end(), REQUIRED_LAYERS.begin(), REQUIRED_LAYERS.end());

		info.enabledLayerCount = static_cast<uint32_t>(layers.size());
		info.ppEnabledLayerNames = layers.data();
		
		info.enabledExtensionCount = static_cast<uint32_t>(_requiredExtensions.size());
		info.ppEnabledExtensionNames = _requiredExtensions.data();

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

		appInfo.applicationVersion = APPLICATION_VERSION;
		appInfo.engineVersion = ENGINE_VERSION;
		appInfo.apiVersion = API_VERSION;
		appInfo.pApplicationName = APPLICATION_NAME;
		appInfo.pEngineName = ENGINE_NAME;

		info.pApplicationInfo = &appInfo;

		checkExtensions();
		checkLayers();

		if (vkCreateInstance(&info, _context.graphics.allocationCallbacks, &_instance) != VK_SUCCESS){
			_context.logger.error("Failed to create vulkan instance");
			throw std::runtime_error("Failed to create vulkan instance");
		}
	}

	void Instance::checkExtensions(){
		std::set<const char*> extensions(REQUIRED_EXTENSIONS.begin(), REQUIRED_EXTENSIONS.end());

		uint32_t extensionsCount;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);

		std::vector<VkExtensionProperties> extensionsProperties(extensionsCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, extensionsProperties.data());

		for(uint32_t i=0; i<extensionsCount; i++){
			auto& properties = extensionsProperties[i];
			extensions.erase(properties.extensionName);
		}
	}

	void Instance::checkLayers(){
		std::set<const char*> layers(REQUIRED_LAYERS.begin(), REQUIRED_LAYERS.end());

		uint32_t layersCount;
		vkEnumerateInstanceLayerProperties(&layersCount, nullptr);

		std::vector<VkLayerProperties> extensionsProperties(layersCount);
		vkEnumerateInstanceLayerProperties(&layersCount, extensionsProperties.data());

		for(uint32_t i=0; i<layersCount; i++){
			auto& properties = extensionsProperties[i];
			layers.erase(properties.layerName);
		}

		if (!layers.empty()){
			_context.logger.warn("Missing %d vulkan instance layer(s) : ", layers.size());
			for (const auto& l : layers){
				_context.logger.warn("\t- %s", l);
			}
			throw std::runtime_error("Missing layer");
		}
	}

	void Instance::checkValidationLayers(){
		uint32_t count;
		vkEnumerateInstanceLayerProperties(&count, nullptr);

		std::vector<VkLayerProperties> availableLayers(count);
		vkEnumerateInstanceLayerProperties(&count, availableLayers.data());

		for (const auto &layerName : REQUIRED_LAYERS){
			bool layerFound = false;

			for (const auto &layerProperties : availableLayers){
				if (strcmp(layerName, layerProperties.layerName) == 0){
					layerFound = true;
					break;
				}
			}

			if (!layerFound){
				_context.logger.warn("Missing vulkan validation layer(s) : %s", layerName);
				throw std::runtime_error("Missing validation layer");
			}
		}
	}

	void Instance::requireGraphicsExtensions(){
		auto extensions = _context.window.vulkanExtensions();
		_requiredExtensions.insert(_requiredExtensions.end(), extensions.begin(), extensions.end());
	}

	void Instance::setupDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& info){
		info = {};
		info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		info.pfnUserCallback = debugCallback;
		info.pUserData = &_context;
	}
}