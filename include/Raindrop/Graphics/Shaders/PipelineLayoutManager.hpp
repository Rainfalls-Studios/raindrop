#ifndef __RAINDROP_GRAPHICS_SHADERS_PIPELINE_LAYOUT_MANAGER_HPP__
#define __RAINDROP_GRAPHICS_SHADERS_PIPELINE_LAYOUT_MANAGER_HPP__

#include <Raindrop/Graphics/Shaders/common.hpp>

namespace Raindrop::Graphics::Shaders{
	class PipelineLayoutManager{
		public:
			PipelineLayoutManager(Context& context);
			~PipelineLayoutManager();

			std::shared_ptr<PipelineLayout> create(const PipelineLayoutBuilder& builder);
			std::shared_ptr<PipelineLayout> get(const std::string& name) const;
			
			void clean();
		
		private:
			Context& _context;
			std::unordered_map<std::string, std::shared_ptr<PipelineLayout>> _layouts;
	};
}

#endif