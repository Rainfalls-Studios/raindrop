#ifndef __RAINDROP_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_COLLECTION_HPP__
#define __RAINDROP_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_COLLECTION_HPP__

#include "common.hpp"
#include <Raindrop/Wrappers/SceneWrapper.hpp>
#include <Raindrop/Graphics/RenderSystems/RenderSequence.hpp>

namespace Raindrop::Graphics::RenderSystems{
	class RenderSystemCollection{
		public:
			RenderSystemCollection(const std::initializer_list<std::pair<std::shared_ptr<RenderSystem>, std::string>>& systems, const std::initializer_list<std::pair<RenderSequence, std::string>>& sequences);
			RenderSystemCollection();

			std::shared_ptr<RenderSystem> getRenderSystem(const std::string& name) const;
			std::vector<std::shared_ptr<RenderSystem>> renderSystems() const;

			const RenderSequence* getRenderSequence(const std::string& name) const;
			std::vector<RenderSequence> renderSequences() const;

		private:
			std::unordered_map<std::string, std::shared_ptr<RenderSystem>> _renderSystems;
			std::unordered_map<std::string, RenderSequence> _renderSequences;
	};
}

#endif