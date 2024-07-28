#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Graphics/SimpleRenderer.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <spdlog/spdlog.h>
#include <iostream>

class CustomSceneProperty : public Raindrop::Scenes::Property{
	public:
		int myValue;
};

struct PushConstant{
	int a;
};


class Testbed : public Raindrop::Engine, public Raindrop::Events::Listener{
	public:
		bool closeEvent(const Raindrop::Events::WindowCloseRequest& event){
			_run = false;
			return true;
		}

		Testbed() : Engine(), Listener(){
			Engine::initialize(INIT_EVERYTHING);
			_renderer.initialize(getGraphicsContext());

			Engine::subscribeToEvent<Raindrop::Events::WindowCloseRequest>(this, &Testbed::closeEvent);
		}

		void run(){
			_run = true;
			auto start = std::chrono::steady_clock::now();

			while (_run){
				auto now = std::chrono::steady_clock::now();

				std::chrono::duration<float> duration = now - start;
				Engine::getGraphicsWindow().events();

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