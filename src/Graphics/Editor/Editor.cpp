#include <Raindrop/Graphics/Editor/Editor.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>
#include <Raindrop/Graphics/Editor/ViewportFramebuffer.hpp>
#include <Raindrop/Graphics/Editor/ViewportPanel.hpp>
#include <Raindrop/Graphics/Editor/SceneHierarchyPanel.hpp>
#include <Raindrop/Graphics/Editor/ComponentEditorPanel.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <imgui/imgui_internal.h>

namespace Raindrop::Graphics::Editor{
	Editor::Editor(GraphicsContext& context, Core::Scene::Scene* scene){
		_context = std::make_unique<EditorContext>(context);

		_context->scene = scene;

		_viewport = std::make_unique<ViewportPanel>(*_context);
		_sceneHierarchy = std::make_unique<SceneHierarchyPanel>(*_context);
		_componentEditor = std::make_unique<ComponentEditorPanel>(*_context);

		context.context.registry["EditorSourceDir"] = RAINDROP_EDITOR_DIR.string();

		tinyxml2::XMLDocument doc;
		doc.LoadFile((RAINDROP_EDITOR_DIR / "icons/dark.xml").string().c_str());
		_context->icons.loadFromXML(doc.RootElement()->FirstChildElement("Icons"));

		for (int i=5; i<20; i+=1){
			_context->fonts.load(std::string("Inter Black ") + std::to_string(i), static_cast<float>(i), RAINDROP_EDITOR_DIR / "fonts/inter/Inter-Black.ttf");
			_context->fonts.load(std::string("Inter Bold ") + std::to_string(i), static_cast<float>(i), RAINDROP_EDITOR_DIR / "fonts/inter/Inter-Bold.ttf");
			_context->fonts.load(std::string("Inter ExtraBold ") + std::to_string(i), static_cast<float>(i), RAINDROP_EDITOR_DIR / "fonts/inter/Inter-ExtraBold.ttf");
			_context->fonts.load(std::string("Inter ExtraLight ") + std::to_string(i), static_cast<float>(i), RAINDROP_EDITOR_DIR / "fonts/inter/Inter-ExtraLight.ttf");
			_context->fonts.load(std::string("Inter Light ") + std::to_string(i), static_cast<float>(i), RAINDROP_EDITOR_DIR / "fonts/inter/Inter-Light.ttf");
			_context->fonts.load(std::string("Inter Medium ") + std::to_string(i), static_cast<float>(i), RAINDROP_EDITOR_DIR / "fonts/inter/Inter-Medium.ttf");
			_context->fonts.load(std::string("Inter Regular ") + std::to_string(i), static_cast<float>(i), RAINDROP_EDITOR_DIR / "fonts/inter/Inter-Regular.ttf");
			_context->fonts.load(std::string("Inter SemiBold ") + std::to_string(i), static_cast<float>(i), RAINDROP_EDITOR_DIR / "fonts/inter/Inter-SemiBold.ttf");
			_context->fonts.load(std::string("Inter Thin ") + std::to_string(i), static_cast<float>(i), RAINDROP_EDITOR_DIR / "fonts/inter/Inter-Thin.ttf");
		}

		ImGui::GetIO().FontDefault = _context->fonts.get("Inter Medium 14");
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

			ImGui::DockBuilderDockWindow("Workspace", dock_id_up);
			ImGui::DockBuilderDockWindow("FileExplorer", dock_id_down);

			ImGui::DockBuilderFinish(dockspace_id);
		}
    	ImGui::End();

		_context->camera.update();
		_viewport->update();
		_context->fileExplorer.update();
		_sceneHierarchy->update();
		_componentEditor->update(_context->selectedEntity);

	}

	VkRenderPass Editor::renderPass() const{
		return _context->viewport.renderPass();
	}

	const glm::mat4& Editor::cameraViewProjection() const{
		return _context->camera.viewProjection;
	}
}