// #include <iostream>
// #include <Raindrop/Raindrop.hpp>
// #include <Raindrop_internal/Graphics/Engine.hpp>
// #include <Raindrop/Components/Transformation.hpp>
// #include <Raindrop/Components/Model.hpp>

// #include "config.h"
// #include <SDL3/SDL.h>

// #include "ForwardRenderSystem.hpp"

// void testbed(){
// 	namespace RD = Raindrop;
// 	namespace fs = std::filesystem;f

// 	using fs::current_path;

// 	current_path(PATH);
// 	RD::Raindrop engine;
// 	auto& engine = engine.engine();

// 	engine.subscribeEvent(
// 		"quit",
// 		[&engine]() -> void {
// 			engine.quit();
// 		}
// 	);

// 	engine.subscribeEvent(
// 		"key.down",
// 		[](RD::Key scancode, RD::Key keycode, RD::KeyMod repeat) -> void {
// 			if (keycode == RD::Key::KEY_LCTRL){
// 				if (SDL_GetRelativeMouseMode() == SDL_TRUE){
// 					SDL_SetRelativeMouseMode(SDL_FALSE);
// 				} else {
// 					SDL_SetRelativeMouseMode(SDL_TRUE);
// 				}
// 			}
// 		}
// 	);

// 	auto scene = engine.createScene();

// 	auto renderSystem = engine.engine().createRenderSystem<ForwardRenderSystem>(engine);
// 	renderSystem->bind(scene);
	
// 	{
// 		auto entity = scene.create();
// 		scene.emplaceComponent<Raindrop::Components::Transformation>(entity);
// 		auto& model = scene.emplaceComponent<Raindrop::Components::Model>(entity);

// 		model.setModel(engine.getAsset<Raindrop::Internal::Graphics::Models::Model>("Model", fs::current_path() / "models/sponza/sponza.obj"));
// 	}

// 	Raindrop::Camera camera;

// 	engine.subscribeEvent(
// 		"Engine.swapchain.resized",
// 		[&camera](RD::Maths::uvec2 size) -> void {
// 			camera.setAspectRatio(static_cast<float>(size.x) / static_cast<float>(size.y));
// 			camera.updateProjection();
// 		}
// 	);

// 	engine.subscribeEvent(
// 		"mouse.motion",
// 		[&](RD::Maths::vec2 position, RD::Maths::vec2 relativePosition) -> void {
// 			if (SDL_GetRelativeMouseMode() == SDL_TRUE){
// 				const float sensitivity = 0.1f;

// 				float yaw = relativePosition.x * sensitivity;
// 				float pitch = -relativePosition.y * sensitivity;
				
// 				RD::Maths::quat yawRotation = RD::Maths::angleAxis(RD::Maths::radians(yaw), RD::Maths::normalize(RD::Maths::vec3(0.0f, 1.0f, 0.0f)));
// 				RD::Maths::quat pitchRotation = RD::Maths::angleAxis(RD::Maths::radians(pitch), RD::Maths::normalize(RD::Maths::vec3(1.0f, 0.0f, 0.0f)));

// 				RD::Maths::quat cameraRotation = camera.getRotation();

// 				cameraRotation = yawRotation * cameraRotation;
// 				cameraRotation = cameraRotation * pitchRotation;

// 				cameraRotation = RD::Maths::normalize(cameraRotation);
// 				camera.setRotation(cameraRotation);
// 			}
// 		}
// 	);

// 	engine.subscribeEvent(
// 		"OnTick",
// 			[&]() -> void {
// 			auto& keys = engine.eventManager().keyEvents();
// 			auto& mouse = engine.eventManager().mouseEvents();

// 			using namespace RD::Core::Event;

// 			auto& translation = camera.getTranslation();
// 			auto& rotation = camera.getRotation();

// 			RD::Maths::vec3 forward = RD::Maths::rotate(rotation, RD::Maths::vec3(0.f, 0.f, -1.f));
// 			RD::Maths::vec3 up = RD::Maths::rotate(rotation, RD::Maths::vec3(0.f, 1.f, 0.f));
// 			RD::Maths::vec3 left = RD::Maths::rotate(rotation, RD::Maths::vec3(1.f, 0.f, 0.f));

// 			float factor = 1.f;//0.0005f;

// 			if (keys[KEY_W]){
// 				camera.translate(forward * factor);
// 			}

// 			if (keys[KEY_S]){
// 				camera.translate(-forward * factor);
// 			}

// 			if (keys[KEY_A]){
// 				camera.translate(left * factor);
// 			}

// 			if (keys[KEY_D]){
// 				camera.translate(-left * factor);
// 			}

// 			if (keys[KEY_SPACE]){
// 				camera.translate(up * factor);
// 			}

// 			if (keys[KEY_LSHIFT]){
// 				camera.translate(-up * factor);
// 			}
// 		}
// 	);

// 	engine.subscribeEvent(
// 		"Engine.frame",
// 		[&](VkCommandBuffer commandBuffer) -> void {
// 			auto properties = scene.getProperty<ForwardRenderSceneProperties>();
// 			auto& data = properties->data;
			
// 			data.ambientColor = RD::Maths::vec4(RD::Maths::vec3(0.1), 1.0);
// 			data.viewProjection = camera.getViewProjection();
// 			data.cameraPosition = RD::Maths::vec4(camera.getTranslation() / 2.f, 0.0);

// 			renderSystem->updateScene(commandBuffer, scene);
// 		}
// 	);

// 	engine.subscribeEvent(
// 		"Engine.baseFramebuffer.renderPass",
// 		[&](VkCommandBuffer commandBuffer) -> void {
// 			renderSystem->render(commandBuffer, scene);
// 		}
// 	);

// 	engine.run();
// }


// void test_prograd(){
// 	namespace RD = Raindrop;
// 	namespace fs = std::filesystem;

// 	using fs::current_path;

// 	current_path(PATH);
// 	RD::Raindrop engine;
// 	auto& engine = engine.engine();

// 	auto module = engine.loadModule(RD::Path(PROGRAD_PATH) / "Prograd");

// 	engine.subscribeEvent(
// 		"quit",
// 		[&engine]() -> void {
// 			engine.quit();
// 		}
// 	);

// 	engine.run();
// }

// int main(int argc, char** argv){
// 	std::cout << "==========================================" << std::endl;
// 	std::cout << "=                 TESTBED                =" << std::endl;
// 	std::cout << "==========================================" << std::endl;
// 	std::cout << std::endl;
	
// 	// testbed();
// 	// test_prograd();

// 	std::cout << std::endl;
// 	std::cout << "==========================================" << std::endl;
// 	std::cout << "=                 TESTBED                =" << std::endl;
// 	std::cout << "==========================================" << std::endl;

// 	return 0;
// }

#include <Raindrop/Raindrop.hpp>
#include "config.h"
#include <iostream>

struct PushConstant{
	Raindrop::Maths::mat4 transform;
};

int main(){
	std::filesystem::current_path(PATH);

	Raindrop::Context context = Raindrop::CreateContext();
	context.initialize();

	Raindrop::Event::Subscribe(
		context,
		"quit",
		[&context](){
			context.stop();
		}
	);

	// Create texture
	auto texture = Raindrop::CreateTexture(context);
	texture.setWidth(255);
	texture.setHeight(255);
	texture.setUsage(Raindrop::Texture::Usage::SAMPLED);
	
	Raindrop::Format format = texture.findBestFormat(
		{
			Raindrop::Format::Properties::RED,
			Raindrop::Format::Properties::GREEN,
			Raindrop::Format::Properties::BLUE
		},
		Raindrop::Format::Features::SAMPLED_IMAGE,
		Raindrop::Format::Properties::PACKED
	);

	// Raindrop::Format::Properties::FlagsTemplate()
	
	texture.setFormat(format);
	texture.initialize();
	

	auto textureSubset = Raindrop::CreateTextureSubset(context, texture);
	textureSubset.initialize();
	textureSubset.release();
	
	// create shader

	// create pipeline layout
	Raindrop::Pipeline::Layout pipelineLayout = Raindrop::CreatePipelineLayout(context);
	pipelineLayout.setPushConstant<PushConstant>(Raindrop::Pipeline::Stage::VERTEX_SHADER);

	pipelineLayout.initialize();
	
	Raindrop::RenderPass renderPass = Raindrop::CreateRenderPass(context);
	{

		auto& attachment = renderPass.addAttachment()
			.setFormat(format)
			.setLoadOperation(Raindrop::RenderPass::AttachmentDescription::Operation::CLEAR)
			.setStoreOperation(Raindrop::RenderPass::AttachmentDescription::Operation::STORE)
			.setStencilLoadOperation(Raindrop::RenderPass::AttachmentDescription::Operation::DONT_LOAD)
			.setStencilStoreOperation(Raindrop::RenderPass::AttachmentDescription::Operation::DONT_STORE)
			.setInitialLayout(Raindrop::Texture::Layout::UNDEFINED)
			.setFinalLayout(Raindrop::Texture::Layout::SHADER_READ_ONLY_OPTIMAL);

		auto& subpass = renderPass.addSubpass()
			.addColorAttachment(
				{
					.attachment = attachment,
					.layout = Raindrop::Texture::Layout::COLOR_ATTACHMENT_OPTIMAL
				}
			);
		
		auto& dependency = renderPass.addDependency()
			.setSrcSubpass(Raindrop::RenderPass::Subpass::External)
			.setDstSubpass(subpass)
			.setSrcStage(Raindrop::Pipeline::Stage::COLOR_ATTACHMENT_OUTPUT)
			.setSrcAccess(Raindrop::RenderPass::Access::NONE)
			.setDstStage(Raindrop::Pipeline::Stage::COLOR_ATTACHMENT_OUTPUT)
			.setDstAccess(Raindrop::RenderPass::Access::COLOR_ATTACHMENT_WRITE);

		renderPass.initialize();
	}

	Raindrop::Pipeline pipeline = Raindrop::CreatePipeline(context);
	{
		auto frag = Raindrop::Asset::Load<Raindrop::Pipeline::Shader>(context, "Shader", "shaders/scene/fullscreenQuad/shader.glsl.frag.spv");
		auto vert = Raindrop::Asset::Load<Raindrop::Pipeline::Shader>(context, "Shader", "shaders/scene/fullscreenQuad/shader.glsl.vert.spv");

		auto& colorAttachment = pipeline.addColorAttachment()
			.enableBlending(false)
			.setWriteMask({
				Raindrop::Color::Components::RED,
				Raindrop::Color::Components::GREEN,
				Raindrop::Color::Components::BLUE,
		});

		pipeline.addStage(frag, Raindrop::Pipeline::Shader::Stage::FRAGMENT);
		pipeline.addStage(vert, Raindrop::Pipeline::Shader::Stage::VERTEX);

		auto& viewport = pipeline.addViewport()
			.setX(0.f)
			.setY(0.f)
			.setWidth(255.f)
			.setHeight(255.f)
			.setMaxDepth(1.f)
			.setMinDepth(0.f);
		
		auto& scissor = pipeline.addScissor()
			.setX(0)
			.setY(0)
			.setWidth(255)
			.setHeight(255);
	
		pipeline.setRenderPass(renderPass);
		pipeline.setLayout(pipelineLayout);

		pipeline.initialize();
	}

	{
		Raindrop::GBuffer buffer = Raindrop::CreateGBuffer(context);

		buffer.allocate(
			1024,
			Raindrop::GBuffer::Usage::TRANSFER_SRC,
			Raindrop::GBuffer::Flags::NONE,
			Raindrop::GMemory::Type::Flags::CPU_VISIBLE
		);
	}

	// {
	// 	Raindrop::GMemory::Type type = Raindrop::FindGetGMemoryType(
	// 		context,
	// 		Raindrop::GMemory::Type::Flags::CPU_VISIBLE
	// 	);

	// 	Raindrop::GMemory memory = Raindrop::AllocateGMemory(context, 4096, type);

	// 	memory.free();
	// }

	// Raindrop::Subscribe(context, "OnSwapchainPass",
	// 	[](Raindrop::CommandBuffer& cmd){
			
	// 	}
	// );

	Raindrop::Vertex::IndexBuffer indexBuffer = Raindrop::CreateIndexBuffer(context);
	indexBuffer.allocate(
		Raindrop::Vertex::IndexBuffer::Type::UINT32,
		1000,
		Raindrop::GBuffer::Usage::NONE,
		Raindrop::GBuffer::Flags::NONE,
		{
			Raindrop::GMemory::Type::Flags::CPU_VISIBLE,
			Raindrop::GMemory::Type::Flags::GPU_LOCAL
		}
	);
	
	Raindrop::Start(context);
	
	indexBuffer.free();
	pipeline.release();
	renderPass.release();
	pipelineLayout.release();
	textureSubset.release();
	texture.release();
	
	context.release();
}