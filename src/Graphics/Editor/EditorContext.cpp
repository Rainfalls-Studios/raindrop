#include <Raindrop/Graphics/Editor/EditorContext.hpp>

namespace Raindrop::Graphics::Editor{
	EditorContext::EditorContext(GraphicsContext& context) :
		context{context},
		viewport{*this, 1080, 720},
		fileExplorer{*this},
		icons{*this},
		toolbar{*this},
		selectedEntity{Core::Scene::INVALID_ENTITY_ID, nullptr}{
	}
}