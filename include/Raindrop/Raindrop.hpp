#ifndef __RAINDROP_RAINDROP_HPP__
#define __RAINDROP_RAINDROP_HPP__

#include <Raindrop/common.hpp>
#include <Raindrop/Core/EngineContext.hpp>
#include <Raindrop/Scene.hpp>
#include <Raindrop/Graphics/Renderer.hpp>
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
			Scene _scene;
			Core::EngineContext _context;
			Graphics::Renderer _renderer;

			bool _launched;

			void render();
	};
}

#endif