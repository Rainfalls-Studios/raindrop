#ifndef __RAINDROP_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_COLLECTION_HPP__
#define __RAINDROP_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_COLLECTION_HPP__

#include "common.hpp"
#include <Raindrop/Wrappers/SceneWrapper.hpp>

namespace Raindrop::Graphics::RenderSystems{
	class RenderSystemCollection{
		public:
			RenderSystemCollection(const std::string& name, const std::initializer_list<std::pair<std::shared_ptr<RenderSystem>, std::string>>& systems = {}, const std::string& description = "None");

			const std::string& name() const;
			const std::string& descrition() const;

			std::shared_ptr<RenderSystem> get(const std::string& name) const;
			std::vector<std::shared_ptr<RenderSystem>> systems() const;

		private:
			std::string _name;
			std::string _description;
			std::unordered_map<std::string, std::shared_ptr<RenderSystem>> _renderSystems;
	};
}

#endif