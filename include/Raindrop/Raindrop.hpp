#ifndef __RAINDROP_RAINDROP_HPP__
#define __RAINDROP_RAINDROP_HPP__

#include <Raindrop/common.hpp>
#include <Raindrop/Graphics/Engine.hpp>
#include <Raindrop/components.hpp>

namespace Raindrop{
	class Raindrop{
		public:
			Raindrop();
			~Raindrop();

			void run();

			Scene& scene();
			const Scene& scene() const;

			void setScene(const Scene& scene);

			Scene createScene();

		private:
			std::unique_ptr<Core::Context> _core;
			std::unique_ptr<Graphics::Engine> _renderEngine;
			// Scene _scene;

			bool _launched;

			void render();
	};
}

#endif