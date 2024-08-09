#ifndef __RAINDROP_ENGINE_HPP__
#define __RAINDROP_ENGINE_HPP__

#include "Context.hpp"

namespace Raindrop{
	class Engine{
		public:
			Engine();
			~Engine();

			void initialize();
			void release();

			// CORE

			Context& getContext();
			const Context& getContext() const;

			UUIDRegistry& getUUIDRegistry();

			template<typename T>
			inline std::shared_ptr<T> getObject(const UUID& uuid){
				return getUUIDRegistry().get<T>(uuid);
			}

			inline Scenes::Entity& getEntity(const UUID& uuid){
				return getUUIDRegistry().getEntity(uuid);
			}

			// GRAPHICS

			Graphics::Context& getGraphicsContext();
			const Graphics::Context& getGraphicsContext() const;

			Graphics::Core::Instance& getGraphicsInstance();
			Graphics::Core::PhysicalDevice& getGraphicsPhysicalDevice();
			Graphics::Core::Device& getGraphicsDevice();
			Graphics::Window::Window& getGraphicsWindow();
			Graphics::Window::Surface& getGraphicsWindowSurface();
			Graphics::Window::Swapchain& getGraphicsWindowSwapchain();
			Graphics::Formats& getGraphicsFormats();
			Graphics::ModelLoader& getGraphicsModelLoader();

			std::shared_ptr<Graphics::RenderPass> createGraphicsRenderPass();
			std::shared_ptr<Graphics::Buffer> createGraphicsBuffer();
			std::shared_ptr<Graphics::DescriptorSetLayout> createGraphicsDescriptorSetLayout();
			std::shared_ptr<Graphics::GraphicsPipeline> createGraphicsGraphicsPipeline();
			std::shared_ptr<Graphics::PipelineLayout> createGraphicsPipelineLayout();
			std::shared_ptr<Graphics::ShaderModule> createGraphicsShaderModule();
			std::shared_ptr<Graphics::Memory> createGraphicsMemory();
			std::shared_ptr<Graphics::Image> createGraphicsImage();
			std::shared_ptr<Graphics::ImageView> createGraphicsImageView();
			std::shared_ptr<Graphics::Sampler> createGraphicsSampler();
			std::shared_ptr<Graphics::DescriptorPool> createGraphicsDescriptorPool();
			std::shared_ptr<Graphics::MeshData> createGraphicsMeshData();
			std::shared_ptr<Graphics::Mesh> createGraphicsMesh();

			Graphics::VertexLayout createGraphicsVertexLayout();
			Graphics::ModelLayoutConfig createGraphicsModelLayoutConfig();

			// EVENTS

			Events::Context& getEventsContext();
			const Events::Context& getEventsContext() const;

			template<typename EventType>
			void subscribeToEvent(std::function<bool(const EventType&)> callback){
				getEventsContext().manager.subscribe<EventType>(callback);
			}

			template<typename EventType, typename ClassType>
			void subscribeToEvent(ClassType& instance, bool (ClassType::*memberFunction)(const EventType&)){
				getEventsContext().manager.subscribe<EventType, ClassType>(instance, memberFunction);
			}

			template<typename EventType, typename ClassType>
			void subscribeToEvent(ClassType* instance, bool (ClassType::*memberFunction)(const EventType&)){
				getEventsContext().manager.subscribe<EventType, ClassType>(instance, memberFunction);
			}

			template<typename T>
			void triggerEvent(const T& event){
				getEventsContext().manager.trigger<T>(event);
			}

			// SCENES

			Scenes::Context& getScenesContext();
			const Scenes::Context& getScenesContext() const;

			std::shared_ptr<Scenes::Scene> createScene();

		private:
			std::unique_ptr<Context> _context;
	};
}

#endif