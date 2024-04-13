#ifndef __RAINDROP_TESTBED_FORWARD_RENDER_SYSTEM_HPP__
#define __RAINDROP_TESTBED_FORWARD_RENDER_SYSTEM_HPP__

#include <Raindrop/Raindrop.hpp>
#include "config.h"

struct ForwardRenderSceneProperties : public Raindrop::SceneProperty{
	struct{
		Raindrop::Maths::vec4 ambientColor;
		Raindrop::Maths::mat4 viewProjection;
		Raindrop::Maths::vec4 cameraPosition;
	} data;

	struct{
		std::size_t setID;
	} __internal__;
};

class ForwardRenderSystem : public Raindrop::Graphics::RenderSystem{
	public:
		ForwardRenderSystem(Raindrop::Raindrop& engine);
		~ForwardRenderSystem();

		void bind(Raindrop::SceneWrapper scene);
		void release(Raindrop::SceneWrapper scene);

		void updateScene(VkCommandBuffer commandBuffer, Raindrop::SceneWrapper scene);
		void render(VkCommandBuffer commandBuffer, Raindrop::SceneWrapper scene);

	private:
		Raindrop::Raindrop& _engine;

		void createDescriptorSetLayout();
		void createDescriptorPool();
		void createDescriptorBuffer();

		void destroyDescriptorSetLayout();
		void destroyDescriptorPool();
		void destroyDescriptorBuffer();
		
		void populateInternProperty(ForwardRenderSceneProperties& prop);

		void allocateDescriptorSets();
		void updateDescriptorSets();

		void createPipelineLayout();
		void createShaderModules();
		void createPipeline();

		void destroyPipelineLayout();
		void destroyShaderModules();
		void destroyPipeline();

		void createShaderModule(const std::vector<char>& code, VkShaderModule& module);


		std::vector<VkDescriptorSet> _sets;
		std::deque<std::size_t> _freeSetIDs;

		VkDescriptorSetLayout _descriptorSetLayout;
		VkDescriptorPool _descriptorPool;
		VkBuffer _descriptorsBuffer;
		VkDeviceMemory _descriptorsMemory;

		std::size_t _alignedDescriptorSize;

		Raindrop::Graphics::PipelineLayoutWrapper _pipelineLayout;
		Raindrop::Graphics::GraphicsPipelineWrapper _pipeline;

		VkShaderModule _vertexShader;
		VkShaderModule _fragmentShader;
};

#endif