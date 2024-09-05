#pragma once

#include "../pch.pch"

namespace Raindrop::Graphics::Vulkan::Core{
	class Context;
	
	class Instance{
		public:
			Instance() noexcept;
			~Instance();

			Instance(const Instance&) = delete;
			Instance& operator=(const Instance&) = delete;

			vkb::InstanceBuilder* operator->() noexcept;
			vkb::InstanceBuilder& operator*() noexcept;

			Instance& prepare(Context& context);
			void initialize();
			void release();

			const VkInstance& get() const noexcept;
			const vkb::Instance& getVkb() const noexcept;

			Instance& setAppName(const char* name);
			Instance& setEngineName(const char* name);
			Instance& setAppVersion(uint32_t major, uint32_t minor, uint32_t patch);
			Instance& setEngineVersion(uint32_t major, uint32_t minor, uint32_t patch);
			Instance& requireVulkanVersion(uint32_t version);
			Instance& enableLayer(const char* layer);
			Instance& enableExtension(const char* extension);

			bool isLayerAvailable(const char* layer) const noexcept;
			bool isExtensionAvailable(const char* extension) const noexcept;

		private:
			Context* _context;
			vkb::Instance _instance;
			std::unique_ptr<vkb::InstanceBuilder> _builder;

			vkb::InstanceBuilder& getBuilder();

			void getLayers();
			void getExtensions();

			std::vector<std::string> _availableLayers;
			std::vector<std::string> _availableExtensions;

	};
}