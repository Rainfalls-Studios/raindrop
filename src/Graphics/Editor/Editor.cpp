#include <Raindrop/Graphics/Editor/Editor.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>
#include <Raindrop/Graphics/Editor/ViewportFramebuffer.hpp>
#include <Raindrop/Graphics/Editor/ViewportPanel.hpp>
#include <imgui/imgui_internal.h>

namespace Raindrop::Graphics::Editor{
	Editor::Editor(GraphicsContext& context){
		_context = std::make_unique<EditorContext>(context);
		_viewport = std::make_unique<ViewportPanel>(*_context);
	}

	Editor::~Editor(){

	}

	bool Editor::beginRenderPass(VkCommandBuffer commandBuffer){
		return _context->viewport.beginRenderPass(commandBuffer);
	}

	void Editor::endRenderPass(VkCommandBuffer commandBuffer) {
		_context->viewport.endRenderPass(commandBuffer);
	}

	void Editor::render(){

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		
		ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking);

		ImGui::PopStyleVar(3);

		

		ImGuiID dockspace_id = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking);
		static auto first_time = true;
		if (first_time) {
			first_time = false;
			ImGui::DockBuilderRemoveNode(dockspace_id);
			ImGui::DockBuilderAddNode(dockspace_id);
			ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

			auto dock_id_up = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.5f, nullptr, &dockspace_id);
			auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.5f, nullptr, &dockspace_id);
			ImGui::DockBuilderDockWindow("Viewport", dock_id_up);
			ImGui::DockBuilderDockWindow("FileExplorer", dock_id_down);

			ImGui::DockBuilderFinish(dockspace_id);
		}
    	ImGui::End();

		_viewport->update();
		_context->fileExplorer.update();

	}

	VkRenderPass Editor::renderPass() const{
		return _context->viewport.renderPass();
	}
}
