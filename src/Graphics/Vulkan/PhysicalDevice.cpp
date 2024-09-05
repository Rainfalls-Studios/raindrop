// #include <Raindrop/Graphics/Vulkan/Core/PhysicalDevice.hpp>
// #include <Raindrop/Graphics/Vulkan/Core/Context.hpp>

// #include <cstring>
// #include <spdlog/spdlog.h>

// namespace Raindrop::Graphics::Vulkan::Core{
// 	PhysicalDevice::PhysicalDevice() noexcept: 
// 		_context{nullptr},
// 		_device{},
// 		_selector{}
// 	{}

// 	PhysicalDevice::~PhysicalDevice(){
// 		release();
// 	}
	
// 	PhysicalDevice& PhysicalDevice::prepare(Context& context){
// 		_context = &context;
// 		_selector = std::make_unique<vkb::PhysicalDeviceSelector>(context.getInstance().getVkb());

// 		_selector->prefer_gpu_device_type(vkb::PreferredDeviceType::discrete);
// 		return *this;
// 	}

// 	PhysicalDevice& PhysicalDevice::initialize(){
// 		SPDLOG_LOGGER_TRACE(_context->getLogger(), "Looking for a suitable physical device...");

// 		_selector->allow_any_gpu_device_type(false);
		
// 		auto list = _selector->select_devices();
// 		auto result = _selector->select();

// 		SPDLOG_LOGGER_TRACE(_context->getLogger(), "Found {} GPU(s) :", list.value().size());

// 		if (!result){
// 			SPDLOG_LOGGER_ERROR(_context->getLogger(), "Failed to find a suitable physical device : {}", result.error().message());
// 			throw std::runtime_error("Failed to find a suitable physical device");
// 		}

// 		_device = result.value();
// 		SPDLOG_LOGGER_TRACE(_context->getLogger(), "using first suitable GPU : {}", _device.name);

// 		_selector.reset();
// 		return *this;
// 	}

// 	void PhysicalDevice::release(){
// 		SPDLOG_LOGGER_TRACE(_context->getLogger(), "Releasing physical device...");
		
// 		_selector.reset();
// 		_device = {};
// 		_context = nullptr;
// 	}

// 	const VkPhysicalDevice& PhysicalDevice::get() const noexcept{
// 		return _device.physical_device;
// 	}

// 	const vkb::PhysicalDevice& PhysicalDevice::getVkb() const noexcept{
// 		return _device;
// 	}

// 	vkb::PhysicalDevice& PhysicalDevice::getVkb() noexcept{
// 		return _device;
// 	}


// 	vkb::PhysicalDeviceSelector& PhysicalDevice::getSelector(){
// 		if (!_selector) throw std::runtime_error("The physical device has either already been initialized or not prepared");
// 		return *_selector;
// 	}


// 	PhysicalDevice& PhysicalDevice::requireSurfaceSupport(VkSurfaceKHR surface){
// 		if (!_selector) throw std::runtime_error("The physical device has already been selected");
// 		_selector->set_surface(surface);
// 		_selector->require_present(surface != VK_NULL_HANDLE);
// 		return *this;
// 	}

// 	PhysicalDevice& PhysicalDevice::requireExtension(const char* ext){
// 		if (!_selector) throw std::runtime_error("The physical device has already been selected");
// 		_selector->add_required_extension(ext);
// 		return *this;
// 	}

// 	PhysicalDevice& PhysicalDevice::requireExtensions(const std::vector<const char*>& extensions){
// 		if (!_selector) throw std::runtime_error("The physical device has already been selected");
// 		_selector->add_required_extensions(extensions);
// 		return *this;
// 	}

// 	PhysicalDevice& PhysicalDevice::requireFeatures(const VkPhysicalDeviceFeatures& features){
// 		if (!_selector) throw std::runtime_error("The physical device has already been selected");
// 		_selector->set_required_features(features);
// 		return *this;
// 	}

// 	// PhysicalDevice& PhysicalDevice::require11Features(const VkPhysicalDeviceVulkan11Features& features){
// 	// 	if (!_selector) throw std::runtime_error("The physical device has already been selected");
// 	// 	_selector->set_required_features_11(features);
// 	// 	return *this;
// 	// }

// 	// PhysicalDevice& PhysicalDevice::require12Features(const VkPhysicalDeviceVulkan12Features& features){
// 	// 	if (!_selector) throw std::runtime_error("The physical device has already been selected");
// 	// 	_selector->set_required_features_12(features);
// 	// 	return *this;
// 	// }

// 	// PhysicalDevice& PhysicalDevice::require13Features(const VkPhysicalDeviceVulkan13Features& features){
// 	// 	if (!_selector) throw std::runtime_error("The physical device has already been selected");
// 	// 	_selector->set_required_features_13(features);
// 	// 	return *this;
// 	// }

// 	std::vector<VkQueueFamilyProperties> PhysicalDevice::getQueueFamilyProperties() const{
// 		return _device.get_queue_families();
// 	}
// }
