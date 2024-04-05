#include <Raindrop/Graphics/Wrappers/RenderSystemWrapper.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics::Wrappers{
	template<typename T>
	typename RenderSystemWrapper<T>::RenderSystem& RenderSystemWrapper<T>::get_raw(Context& context, const RenderSystemID& ID){
		return context.renderSystems.getRenderSystem(ID);
	}
}