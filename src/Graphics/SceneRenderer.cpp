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

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Raindrop/Core/Scene/Components/Camera.hpp>
#include <Raindrop/Core/Scene/Components/Hierarchy.hpp>
#include <Raindrop/Core/Scene/Components/Tag.hpp>
#include <Raindrop/Core/Scene/Components/Transform.hpp>
#include <Raindrop/Graphics/Components/Model.hpp>

namespace Raindrop::Graphics{
	SceneRenderer::SceneRenderer(GraphicsContext& context) : _context{context}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.SceneRenderer");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		CLOG(INFO, "Engine.Graphics.SceneRenderer") << "Creating scene renderer ...";

		createDescriptorPool();
		createSetLayout();
		createDescriptorSet();

		CLOG(INFO, "Engine.Graphcis.SceneRenderer") << "Scene renderer created with success !";
	}

	SceneRenderer::~SceneRenderer(){
		CLOG(INFO, "Engine.Graphics.SceneRenderer") << "Destroying scene renderer ...";
		
		_setLayout.reset();
		vkResetDescriptorPool(_context.device.get(), _descriptorPool->get(), 0);
		_descriptorPool.reset();

		CLOG(INFO, "Engine.Graphcis.SceneRenderer") << "Scene renderer destroyed with success !";
	}

	void SceneRenderer::drawEntity(Core::Scene::Entity entity, VkPipelineLayout layout, VkCommandBuffer commandBuffer, glm::mat4& viewTransform){
		auto& transform = entity.transform();

		glm::mat4 rotationMatrix = glm::mat4_cast(transform.rotation);
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), transform.translation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), transform.scale);

		PushConstant p;
		p.viewTransform = viewTransform;
		p.localTransform = translationMatrix * rotationMatrix * scaleMatrix;

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
				m->draw(commandBuffer);
			}
		}

		for (auto c : entity){
			drawEntity(c, layout, commandBuffer, viewTransform);
		}
	}
	
	void SceneRenderer::createDescriptorPool(){
		Builders::DescriptorPoolBuilder builder;
		builder.setMaxSets(1);
		builder.pushPoolSize({VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1});

		_descriptorPool = builder.build(_context);
	}

	void SceneRenderer::createSetLayout(){
		Builders::DescriptorSetLayoutBuilder builder;

		VkDescriptorSetLayoutBinding binding;
		binding.binding = 0;
		binding.descriptorCount = 1;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		binding.pImmutableSamplers = nullptr;
		builder.pushBinding(binding);

		_setLayout = builder.build(_context);
	}

	void SceneRenderer::createDescriptorSet(){
		VkDescriptorSetAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.descriptorSetCount = 1;
		info.descriptorPool = _descriptorPool->get();
		
		VkDescriptorSetLayout layout = _setLayout->get();
		info.pSetLayouts = &layout;
		
		if (vkAllocateDescriptorSets(_context.device.get(), &info, &_descriptorSet) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.WorldFramebuffer") << "Failed to create world framebuffer descriptor set";
			throw std::runtime_error("Failed to create world framebuffer descriptor set");
		}
	}
}