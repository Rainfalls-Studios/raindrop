#include <Raindrop/Raindrop.hpp>
#include <spdlog/spdlog.h>

class Renderer{
	public:
		Renderer() : 
			_context{nullptr}
		{}

		~Renderer(){
			mainCommandPool.release();
		}

		void initialize(Raindrop::Graphics::Context& context){
			_context = &context;
			mainCommandPool.prepare(*_context);
			mainCommandPool.setCreateFlags(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
			mainCommandPool.setQueueFamily(_context->getDevice().graphicsQueue.familyIndex);

			mainCommandPool.initialize();
		}

	private:
		Raindrop::Graphics::Context* _context;
		Raindrop::Graphics::CommandPool mainCommandPool;
		
};

class Testbed : public Raindrop::Engine{
	public:
		Testbed() : Engine(){
			Engine::initialize(INIT_EVERYTHING);
			_renderer.initialize(getGraphicsContext());
		}

		void run(){
			auto& graphics = Engine::getGraphicsContext();
		
		}

	private:
		Renderer _renderer;
};

int main(){
	Testbed testbed;

	return EXIT_SUCCESS;
}