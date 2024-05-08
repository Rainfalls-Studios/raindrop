#ifndef __RAINDROP_INTERNAL_GRAPHICS_CORE_INSTANCE_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_CORE_INSTANCE_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics::Core{
	class Instance{
		public:
			static constexpr const char* APPLICATION_NAME = "Raindrop";
			static constexpr const char* ENGINE_NAME = "Raindrop Engine";
			static constexpr uint32_t APPLICATION_VERSION = VK_MAKE_API_VERSION(0, 0, 1, 0);
			static constexpr uint32_t ENGINE_VERSION = VK_MAKE_API_VERSION(0, 0, 1, 0);
			static constexpr uint32_t API_VERSION = VK_API_VERSION_1_2;

			static constexpr const char* REQUIRED_LAYERS[] = {};
			static constexpr const char* REQUIRED_EXTENSIONS[] = {
				#ifndef NDEBUG
					"VK_EXT_debug_utils"
				#endif
			};
			
			static constexpr const char* REQUIRED_VALIDATION_LAYERS[] = {"VK_LAYER_KHRONOS_validation"};

			static constexpr std::size_t REQUIRED_LAYERS_COUNT = sizeof(REQUIRED_LAYERS) / sizeof(REQUIRED_LAYERS[0]);
			static constexpr std::size_t REQUIRED_EXTENSIONS_COUNT = sizeof(REQUIRED_EXTENSIONS) / sizeof(REQUIRED_EXTENSIONS[0]);
			static constexpr std::size_t REQUIRED_VALIDATION_LAYERS_COUNT = sizeof(REQUIRED_VALIDATION_LAYERS) / sizeof(REQUIRED_VALIDATION_LAYERS[0]);
			
			#if defined(NDEBUG)
				static constexpr bool REQUIRE_VALIDATION = false;
			#else
				static constexpr bool REQUIRE_VALIDATION = true;
			#endif


			Instance(Context& context);
			~Instance();

			Instance(const Instance&) = delete;
			Instance& operator=(const Instance&) = delete;

			VkInstance get() const;

		private:
			Context& _context;
			VkInstance _instance = VK_NULL_HANDLE;

			std::vector<const char*> _requiredExtensions;

			void build();
			void checkExtensions();
			void checkLayers();
			void checkValidationLayers();
			void setupDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& info);
			void requireGraphicsExtensions();

	};
}

#endif