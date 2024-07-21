#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Graphics/SimpleRenderer.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <spdlog/spdlog.h>
#include <iostream>


class Testbed : public Raindrop::Engine, public Raindrop::Events::Listener{
	public:
		bool closeEvent(const Raindrop::Events::WindowCloseRequest& event){
			_run = false;
			return true;
		}

		Testbed() : Engine(), Listener(){
			Engine::initialize(INIT_EVERYTHING | INIT_EVENTS);
			_renderer.initialize(getGraphicsContext());

			auto& eventManager = Engine::getEventsContext().manager;
			eventManager.subscribe<Raindrop::Events::WindowCloseRequest>(this, &Testbed::closeEvent);
		}

		void run(){
			_run = true;
			auto start = std::chrono::steady_clock::now();

			while (_run){
				auto now = std::chrono::steady_clock::now();

				std::chrono::duration<float> duration = now - start;
				getGraphicsContext().window.window.events();


				auto commandBuffer = _renderer.beginFrame();
				if (commandBuffer){

					glm::vec3 clearColor;
					clearColor.r = glm::sin(duration.count());
					clearColor.g = glm::cos(duration.count() * 3.14);
					clearColor.b = glm::sin(duration.count() * 1.5);

					_renderer.setSwapchainColor(clearColor);
					
					_renderer.beginSwapchainRenderPass(commandBuffer);
					_renderer.endSwapchainRenderPass(commandBuffer);

					_renderer.endFrame(commandBuffer);
				}
			}
		}

	private:
		Raindrop::Graphics::SimpleRenderer _renderer;
		bool _run;
};

int main(){
	Testbed testbed;

	testbed.run();

	return EXIT_SUCCESS;
}