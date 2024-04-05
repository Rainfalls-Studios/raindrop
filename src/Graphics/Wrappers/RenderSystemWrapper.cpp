#include <Raindrop/Graphics/Wrappers/RenderSystemWrapper.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics::Wrappers{
	using RenderSystem = RenderSystems::RenderSystem;
	using RenderSystemID = RenderSystems::RenderSystemID;

    RenderSystem& ___get_render_system___(Context& context, const RenderSystemID& ID) {
        auto system = context.renderSystems.get(ID);
        
        if (system == nullptr){
            throw std::runtime_error("Invalid render system");
        }

        return *system;
    }
}