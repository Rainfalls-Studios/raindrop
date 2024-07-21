#ifndef __RAINDROP_ENGINE_HPP__
#define __RAINDROP_ENGINE_HPP__

#include "Context.hpp"
#include "Graphics/Context.hpp"
#include "Events/Context.hpp"
#include "Scenes/Context.hpp"

namespace Raindrop{
	class Engine{
		public:
			Engine();
			~Engine();

			enum InitFlagBits{
				INIT_GRAPHICS = 1 << 0,
				INIT_EVENTS = 1 << 1,
				INIT_SCENES = 1 << 2,

				INIT_EVERYTHING = INIT_GRAPHICS | INIT_EVENTS | INIT_SCENES
			};

			using InitFlags = std::uint32_t;

			void initialize(InitFlags flags = 0);

			void release();

			// CORE

			Context& getContext();
			const Context& getContext() const;

			// GRAPHICS

			Graphics::Context& getGraphicsContext();
			const Graphics::Context& getGraphicsContext() const;

			// EVENTS

			Events::Context& getEventsContext();
			const Events::Context& getEventsContext() const;

			// SCENES

			Scenes::Context& getScenesContext();
			const Scenes::Context& getScenesContext() const;

		private:
			std::unique_ptr<Context> _context;
			std::unique_ptr<Graphics::Context> _graphics;
			std::unique_ptr<Events::Context> _events;
			std::unique_ptr<Scenes::Context> _scenes;

			void initializeCore();
			void initializeGraphics();
			void initializeEvents();
			void initializeScenes();
	};
}

#endif