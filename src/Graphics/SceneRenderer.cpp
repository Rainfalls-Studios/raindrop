#include <Raindrop/Graphics/SceneRenderer.hpp>
#include <Raindrop/Graphics/DescriptorPool.hpp>
#include <Raindrop/Graphics/DescriptorSetLayout.hpp>
#include <Raindrop/Graphics/builders/DescriptorPoolBuilder.hpp>
#include <Raindrop/Graphics/builders/DescriptorSetLayoutBuilder.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Core/Scene/Entity.hpp>
#include <Raindrop/Graphics/Texture.hpp>
#include <Raindrop/Graphics/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Model.hpp>
#include <Raindrop/Graphics/FrameState.hpp>

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Raindrop/Core/Scene/Components/Hierarchy.hpp>
#include <Raindrop/Core/Scene/Components/Tag.hpp>
#include <Raindrop/Core/Scene/Components/Transform.hpp>
#include <Raindrop/Graphics/Components/Camera.hpp>
#include <Raindrop/Graphics/Components/Model.hpp>

namespace Raindrop::Graphics{
	SceneRenderer::SceneRenderer(GraphicsContext& context) : _context{context}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.SceneRenderer");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		CLOG(INFO, "Engine.Graphics.SceneRenderer") << "Creating scene renderer ...";

		CLOG(INFO, "Engine.Graphcis.SceneRenderer") << "Scene renderer created with success !";
	}

	SceneRenderer::~SceneRenderer(){
		CLOG(INFO, "Engine.Graphics.SceneRenderer") << "Destroying scene renderer ...";

		CLOG(INFO, "Engine.Graphcis.SceneRenderer") << "Scene renderer destroyed with success !";
	}

	void SceneRenderer::renderWithTextures(Core::Scene::Entity entity, const RenderWithTextureInfo& info){
		auto& viewTransform = info.viewTransform;
		auto& commandBuffer = info.commandBuffer;
		auto& layout = info.layout;

		auto& transform = entity.transform();

		glm::mat4 rotationMatrix = glm::mat4_cast(transform.rotation);
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), transform.translation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), transform.scale);

		PushConstant p;
		p.viewTransform = viewTransform;
		p.localTransform = translationMatrix * rotationMatrix * scaleMatrix;

		FrameState frameState;
		frameState.commandBuffer = commandBuffer;

		if (entity.hasComponent<Components::Model>()){
			auto& model = entity.getComponent<Components::Model>();

			auto texture = model.texture();
			auto m = model.model();
			VkDescriptorSet descriptorSet = model.descriptorSet();

			if (m){
				vkCmdBindDescriptorSets(
					commandBuffer,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					layout,
					0,
					1,
					&descriptorSet,
					0,
					nullptr
				);

				vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &p);
				m->draw(frameState);
			}
		}

		for (auto c : entity){
			renderWithTextures(c, info);
		}
	}

	void SceneRenderer::renderForDepthMap(Core::Scene::Entity entity, const RenderForDepthMapInfo& info){
		auto& viewTransform = info.viewTransform;
		auto& commandBuffer = info.commandBuffer;
		auto& layout = info.layout;

		auto& transform = entity.transform();

		glm::mat4 rotationMatrix = glm::mat4_cast(transform.rotation);
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), transform.translation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), transform.scale);

		PushConstant p;
		p.viewTransform = viewTransform;
		p.localTransform = translationMatrix * rotationMatrix * scaleMatrix;

		FrameState frameState;
		frameState.commandBuffer = commandBuffer;

		if (entity.hasComponent<Components::Model>()){
			auto& model = entity.getComponent<Components::Model>();

			auto m = model.model();
			VkDescriptorSet descriptorSet = model.descriptorSet();

			if (m){
				vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &p);
				m->draw(frameState);
			}
		}

		for (auto c : entity){
			renderForDepthMap(c, info);
		}
	}
}