#ifndef __RAINDROP_RAINDROP_HPP__
#define __RAINDROP_RAINDROP_HPP__

#include <Raindrop/common.hpp>
#include <Raindrop/Core/common.hpp>
#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>
#include <Raindrop/Scene.hpp>
#include <Raindrop/Asset.hpp>
#include <Raindrop/Event.hpp>

namespace Raindrop{
	class Raindrop{
		public:
			Raindrop();
			~Raindrop();

			void run();

			void triggerEvent(const std::string& event);
			void subscribeEvent(const std::string& event, const EventCallback& callback);

			Scene createScene(const std::string& name);
			Scene createOrGetScene(const std::string& name);
			Scene getScene(const std::string& name);
			void removeScene(const std::string& name);
			void removeScene(const Scene& scene);

			Asset loadOrGet(const std::string& path);

			// ! TEMP
			Core::Registry::Registry& registry();
			
			void exit();

			Core::Engine& core();
			Graphics::Engine& graphics();

		private:
			std::unique_ptr<Core::Engine> _core;
			std::unique_ptr<Graphics::Engine> _graphics;

			bool _launched;
	};
}

#endif