#include <Raindrop/Graphics/Core/PhysicalDevice.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

#include <cstring>
#include <spdlog/spdlog.h>


namespace Raindrop::Graphics::Core{
	PhysicalDevice::PhysicalDevice() noexcept: 
		_context{nullptr},
		_device{},
		_selector{}
	{}

	PhysicalDevice::~PhysicalDevice(){
		release();
	}
	
	PhysicalDevice& PhysicalDevice::prepare(Context& context){
		_context = &context;
		_selector = std::make_unique<vkb::PhysicalDeviceSelector>(context.instance.getVkb());

		_selector->prefer_gpu_device_type(vkb::PreferredDeviceType::discrete);
		return *this;
	}

	PhysicalDevice& PhysicalDevice::initialize(){
		_context->logger->info("Looking for suitable physical device...");

		auto list = _selector->select_devices();
		
		auto result = _selector->select();

		_context->logger->info("Found {} GPU(s) :", list.value().size());
		for (auto& d : list.value()){
			_context->logger->info("\t - {}", d.name);
		}

		if (!result){
			_context->logger->error("Failed to find a suitable physical device : {}", result.error().message());
			throw std::runtime_error("Failed to find a suitable physical device");
		}

		_device = result.value();
		_context->logger->info("using first suitable GPU : {}", _device.name);

		_selector.reset();
		return *this;
	}

	void PhysicalDevice::release(){
		_context->logger->info("Releasing physical device...");
		
		_selector.reset();
		_device = {};
		_context = nullptr;
	}

	const VkPhysicalDevice& PhysicalDevice::get() const noexcept{
		return _device.physical_device;
	}

	const vkb::PhysicalDevice& PhysicalDevice::getVkb() const noexcept{
		return _device;
	}

	PhysicalDevice& PhysicalDevice::requireSurfaceSupport(VkSurfaceKHR surface){
		if (!_selector) throw std::runtime_error("The physical device has already been selected");
		_selector->set_surface(surface);
		_selector->require_present(surface != VK_NULL_HANDLE);
		return *this;
	}

	PhysicalDevice& PhysicalDevice::requireExtension(const char* ext){
		if (!_selector) throw std::runtime_error("The physical device has already been selected");
		_selector->add_required_extension(ext);
		return *this;
	}

	PhysicalDevice& PhysicalDevice::requireExtensions(const std::vector<const char*>& extensions){
		if (!_selector) throw std::runtime_error("The physical device has already been selected");
		_selector->add_required_extensions(extensions);
		return *this;
	}

	PhysicalDevice& PhysicalDevice::requireFeatures(VkPhysicalDeviceFeatures& features){
		if (!_selector) throw std::runtime_error("The physical device has already been selected");
		_selector->set_required_features(features);
		return *this;
	}

	PhysicalDevice& PhysicalDevice::require11Features(VkPhysicalDeviceVulkan11Features& features){
		if (!_selector) throw std::runtime_error("The physical device has already been selected");
		_selector->set_required_features_11(features);
		return *this;
	}

	PhysicalDevice& PhysicalDevice::require12Features(VkPhysicalDeviceVulkan12Features& features){
		if (!_selector) throw std::runtime_error("The physical device has already been selected");
		_selector->set_required_features_12(features);
		return *this;
	}

	PhysicalDevice& PhysicalDevice::require13Features(VkPhysicalDeviceVulkan13Features& features){
		if (!_selector) throw std::runtime_error("The physical device has already been selected");
		_selector->set_required_features_13(features);
		return *this;
	}

	std::vector<VkQueueFamilyProperties> PhysicalDevice::getQueueFamilyProperties() const{
		return _device.get_queue_families();
	}

}
