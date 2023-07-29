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
			const glm::mat4& cameraViewProjection() const;
			const glm::vec3& cameraDirection() const;
			const glm::vec3& cameraPosition() const;

		private:
			std::unique_ptr<EditorContext> _context;
			std::unique_ptr<WorkspacePanel> _workspace;

			void menuBar();

			void fileMenu();
			void editorMenu();
			void sceneMenu();
			void assetsMenu();

	};
}

#endif