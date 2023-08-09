#include <Raindrop/Graphics/ForwardShader/ForwardShader.hpp>
#include <Raindrop/Graphics/ForwardShader/Framebuffer.hpp>
#include <Raindrop/Graphics/ForwardShader/DescriptorSet.hpp>
#include <Raindrop/Graphics/ForwardShader/Pipelines/Pipelines.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/GraphicsPipeline.hpp>

#include <Raindrop/Graphics/Components/Sun.hpp>
#include <Raindrop/Graphics/Components/LightPoint.hpp>
#include <Raindrop/Graphics/Components/Spotlight.hpp>
#include <Raindrop/Core/Scene/Components/Transform.hpp>

#include <Raindrop/Graphics/ForwardShader/PushContants/LightPoint.hpp>
#include <Raindrop/Graphics/ForwardShader/PushContants/ShadowLightPoint.hpp>
#include <Raindrop/Graphics/ForwardShader/PushContants/ShadowSpotlight.hpp>
#include <Raindrop/Graphics/ForwardShader/PushContants/ShadowSun.hpp>
#include <Raindrop/Graphics/ForwardShader/PushContants/Spotlight.hpp>
#include <Raindrop/Graphics/ForwardShader/PushContants/Sun.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Raindrop::Graphics::ForwardShader{
	ForwardShader::ForwardShader(GraphicsContext& context) : _context{context}{
		_descriptorSet = std::make_unique<DescriptorSet>(_context);
		_framebuffer = std::make_unique<Framebuffer>(_context, 1080, 720);
		_pipelines = std::make_unique<Pipelines::Pipelines>(_context, _descriptorSet->layout());
	}

	ForwardShader::~ForwardShader(){
		_pipelines.reset();
		_framebuffer.reset();
		_descriptorSet.reset();
	}

	void ForwardShader::updateDescriptor(WorldFramebuffer &worldFramebuffer){
		_descriptorSet->update(worldFramebuffer);
	}

	VkDescriptorImageInfo ForwardShader::attachmentInfo() const{
		return _framebuffer->attachmentInfo();
	}

	void ForwardShader::render(VkCommandBuffer commandBuffer, const glm::vec3& cameraPosition, const glm::vec3& cameraDirection){
		_framebuffer->beginRenderPass(commandBuffer);

		renderLightPoints(commandBuffer, cameraPosition, cameraDirection);
		renderSpotlights(commandBuffer, cameraPosition, cameraDirection);
		renderSuns(commandBuffer, cameraPosition, cameraDirection);

		_framebuffer->endRenderPass(commandBuffer);
	}

	void ForwardShader::renderLightPoints(VkCommandBuffer commandBuffer, const glm::vec3& cameraPosition, const glm::vec3& cameraDirection){
		auto lights = _context.scene.componentEntities<Components::LightPoint>();
		if (lights.empty()) return;

		auto& pipeline = _pipelines->lightPoint.pipeline();
		auto set = _descriptorSet->set();

		pipeline.bind(commandBuffer);

		vkCmdBindDescriptorSets(
			commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipeline.layout(),
			0,
			1,
			&set,
			0,
			nullptr
		);

		PushConstants::LightPoint p;

		p.cameraDirection = cameraDirection;
		p.cameraPosition = cameraPosition;

		for (const auto& l : lights){
			auto& transform = _context.scene.getComponent<Core::Scene::Components::Transform>(l);
			auto& light = _context.scene.getComponent<Components::LightPoint>(l);

			p.position = transform.translation;
			p.color = light.color;
			p.intensity = light.intensity;

			vkCmdPushConstants(
				commandBuffer,
				pipeline.layout(),
				VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(PushConstants::LightPoint),
				&p
			);

			vkCmdDraw(commandBuffer, 6, 1, 0, 0);
		}
	}

	void ForwardShader::renderSpotlights(VkCommandBuffer commandBuffer, const glm::vec3& cameraPosition, const glm::vec3& cameraDirection){
		auto lights = _context.scene.componentEntities<Components::Spotlight>();
		if (lights.empty()) return;

		auto& pipeline = _pipelines->spotlight.pipeline();
		auto set = _descriptorSet->set();

		pipeline.bind(commandBuffer);

		vkCmdBindDescriptorSets(
			commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipeline.layout(),
			0,
			1,
			&set,
			0,
			nullptr
		);

		PushConstants::Spotlight p;

		p.cameraDirection = cameraDirection;
		p.cameraPosition = cameraPosition;

		for (const auto& l : lights){
			auto& transform = _context.scene.getComponent<Core::Scene::Components::Transform>(l);
			auto& light = _context.scene.getComponent<Components::Spotlight>(l);

			p.position = transform.translation;
			p.color = light.color;
			p.intensity = light.intensity;
			p.cutOff = light.cutOff;
			p.outerCutOff = light.outerCutOff;
			
			p.direction = glm::rotate(transform.rotation, glm::vec3(0.0f, 0.0f, -1.0f));
			p.direction = glm::normalize(p.direction);

			vkCmdPushConstants(
				commandBuffer,
				pipeline.layout(),
				VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(PushConstants::Spotlight),
				&p
			);
			
			vkCmdDraw(commandBuffer, 6, 1, 0, 0);
		}
	}

	void ForwardShader::renderSuns(VkCommandBuffer commandBuffer, const glm::vec3& cameraPosition, const glm::vec3& cameraDirection){
		auto lights = _context.scene.componentEntities<Components::Sun>();
		if (lights.empty()) return;

		auto& pipeline = _pipelines->sun.pipeline();
		auto set = _descriptorSet->set();

		pipeline.bind(commandBuffer);

		vkCmdBindDescriptorSets(
			commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipeline.layout(),
			0,
			1,
			&set,
			0,
			nullptr
		);

		PushConstants::Sun p;

		p.cameraDirection = cameraDirection;
		p.cameraPosition = cameraPosition;

		for (const auto& l : lights){
			auto& transform = _context.scene.getComponent<Core::Scene::Components::Transform>(l);
			auto& light = _context.scene.getComponent<Components::Sun>(l);

			p.color = light.color();
			p.direction = glm::rotate(transform.rotation, glm::vec3(0.0f, 0.0f, -1.0f));
			p.direction = glm::normalize(p.direction);
			p.intensity = light.intensity();

			vkCmdPushConstants(
				commandBuffer,
				pipeline.layout(),
				VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(PushConstants::Sun),
				&p
			);
			
			vkCmdDraw(commandBuffer, 6, 1, 0, 0);
		}
	}
}