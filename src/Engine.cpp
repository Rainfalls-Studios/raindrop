#include <Raindrop/Engine.hpp>
#include <Raindrop/Graphics/common.hpp>

namespace Raindrop{
	Engine::Engine() :
		_context{nullptr},
		_graphics{nullptr}
	{}

	Engine::~Engine(){
		release();
	}

	void Engine::initialize(InitFlags flags){
		initializeCore();

		if (flags & INIT_EVENTS){
			initializeEvents();
		}

		if (flags & INIT_GRAPHICS){
			initializeGraphics();
		}

		if (flags & INIT_SCENES){
			initializeScenes();
		}
	}

	void Engine::release(){
		_scenes.reset();
		_graphics.reset();
		_events.reset();
		_context.reset();
	}

	void Engine::initializeCore(){
		_context = std::make_unique<Context>();

	}

	void Engine::initializeGraphics(){
		_graphics = std::make_unique<Graphics::Context>();
		_graphics->initialize(getEventsContext());

		// _graphics->core.createLogger();
		// _graphics->core.instance.initialize(_graphics->core);
	}

	void Engine::initializeEvents(){
		_events = std::make_unique<Events::Context>();
	}	
	
	void Engine::initializeScenes(){
		_scenes = std::make_unique<Scenes::Context>();
	}


	// ======================= CORE ==============================

	Context& Engine::getContext(){
		if (!_context){
			throw std::runtime_error("Core has not been initialized");
		}
		return *_context;
	}

	const Context& Engine::getContext() const{
		if (!_context){
			throw std::runtime_error("Core has not been initialized");
		}
		return *_context;
	}

	// ==================== GRAPHICS ============================

	Graphics::Context& Engine::getGraphicsContext(){
		if (!_graphics){
			throw std::runtime_error("Graphics context has not been initialized");
		}
		return *_graphics;
	}

	const Graphics::Context& Engine::getGraphicsContext() const{
		if (!_graphics){
			throw std::runtime_error("Graphics context has not been initialized");
		}
		return *_graphics;
	}


	Graphics::Core::Instance& Engine::getGraphicsInstance(){
		return getGraphicsContext().core.instance;
	}

	Graphics::Core::PhysicalDevice& Engine::getGraphicsPhysicalDevice(){
		return getGraphicsContext().core.physicalDevice;
	}

	Graphics::Core::Device& Engine::getGraphicsDevice(){
		return getGraphicsContext().core.device;
	}

	Graphics::Window::Window& Engine::getGraphicsWindow(){
		return getGraphicsContext().window.window;
	}

	Graphics::Window::Surface& Engine::getGraphicsWindowSurface(){
		return getGraphicsContext().window.surface;
	}

	Graphics::Window::Swapchain& Engine::getGraphicsWindowSwapchain(){
		return getGraphicsContext().window.swapchain;
	}

	Graphics::Formats& Engine::getGraphicsFormats(){
		return getGraphicsContext().formats;
	}

	Graphics::RenderPass Engine::createGraphicsRenderPass(){
		Graphics::RenderPass renderPass;
		renderPass.prepare(getGraphicsContext());
		return std::move(renderPass);
	}

	Graphics::Buffer Engine::createGraphicsBuffer(){
		Graphics::Buffer buffer;
		buffer.prepare(getGraphicsContext());
		return std::move(buffer);
	}

	Graphics::DescriptorSetLayout Engine::createGraphicsDescriptorSetLayout(){
		Graphics::DescriptorSetLayout descriptorSetLayout;
		descriptorSetLayout.prepare(getGraphicsContext());
		return std::move(descriptorSetLayout);
	}

	Graphics::GraphicsPipeline Engine::createGraphicsGraphicsPipeline(){
		Graphics::GraphicsPipeline pipeline;
		pipeline.prepare(getGraphicsContext());
		return std::move(pipeline);
	}

	Graphics::PipelineLayout Engine::createGraphicsPipelineLayout(){
		Graphics::PipelineLayout layout;
		layout.prepare(getGraphicsContext());
		return std::move(layout);
	}

	Graphics::ShaderModule Engine::createGraphicsShaderModule(){
		Graphics::ShaderModule module;
		module.prepare(getGraphicsContext());
		return std::move(module);
	}

	Graphics::Memory Engine::createGraphicsMemory(){
		Graphics::Memory memory;
		memory.prepare(getGraphicsContext());
		return std::move(memory);
	}

	Graphics::Image Engine::createGraphicsImage(){
		Graphics::Image image;
		image.prepare(getGraphicsContext());
		return std::move(image);
	}

	Graphics::ImageView Engine::createGraphicsImageView(){
		Graphics::ImageView imageView;
		imageView.prepare(getGraphicsContext());
		return std::move(imageView);
	}

	Graphics::Sampler Engine::createGraphicsSampler(){
		Graphics::Sampler sampler;
		sampler.prepare(getGraphicsContext());
		return std::move(sampler);
	}

	Graphics::DescriptorPool Engine::createGraphicsDescriptorPool(){
		Graphics::DescriptorPool pool;
		pool.prepare(getGraphicsContext());
		return std::move(pool);
	}

	Graphics::MeshData Engine::createGraphicsMeshData(){
		Graphics::MeshData mesh;
		mesh.prepare(getGraphicsContext());
		return std::move(mesh);
	}

	// ==================== EVENTS ============================

	Events::Context& Engine::getEventsContext(){
		if (!_events){
			throw std::runtime_error("Events context has not been initialized");
		}
		return *_events;
	}

	const Events::Context& Engine::getEventsContext() const{
		if (!_events){
			throw std::runtime_error("Events context has not been initialized");
		}
		return *_events;
	}


	// ==================== SCENES ============================

	Scenes::Context& Engine::getScenesContext(){
		if (!_scenes){
			throw std::runtime_error("Scenes context has not been initialized");
		}
		return *_scenes;
	}

	const Scenes::Context& Engine::getScenesContext() const{
		if (!_scenes){
			throw std::runtime_error("Scenes context has not been initialized");
		}
		return *_scenes;
	}

	Scenes::Scene Engine::createScene(){
		Scenes::Scene scene;
		scene.prepare(getScenesContext());
		return std::move(scene);
	}
}