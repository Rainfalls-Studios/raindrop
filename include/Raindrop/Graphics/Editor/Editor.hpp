#ifndef __RAINDROP_GRAPHICS_EDITOR_EDITOR_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_EDITOR_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>
#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Graphics::Editor{
	class Editor{
		public:
			Editor(GraphicsContext& context, Core::Scene::Scene* scene);
			~Editor();

			bool beginRenderPass(VkCommandBuffer commandBuffer);
			void endRenderPass(VkCommandBuffer commandBuffer);

			void render();

			VkRenderPass renderPass() const;

		private:
			std::unique_ptr<EditorContext> _context;
			std::unique_ptr<ViewportPanel> _viewport;
			std::unique_ptr<SceneHierarchyPanel> _sceneHierarchy;

	};
}

#endif