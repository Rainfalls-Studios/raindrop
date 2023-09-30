#ifndef __RAINDROP_GRAPHICS_RENDER_PASSES_Manager_HPP__
#define __RAINDROP_GRAPHICS_RENDER_PASSES_Manager_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class RenderPassesManager{
		public:
			RenderPassesManager(GraphicsContext& context);
			~RenderPassesManager();

			std::weak_ptr<RenderPass> get(const std::string& name);
			std::weak_ptr<RenderPass> operator[](const std::string& name);

			void set(const std::string& name, const std::shared_ptr<RenderPass>& renderPass);

			template<typename T, typename... Args>
			void set(const std::string& name, Args&&... args){
				set(name, std::make_shared<T>(std::forward<Args>(args)...));
			}

			void destroy(const std::string& name);
			bool has(const std::string& name) const;

		private:
			GraphicsContext& _context;

			struct RenderPassData{
				std::shared_ptr<RenderPass> renderPass;
			};

			std::unordered_map<std::string, std::shared_ptr<RenderPass>> _nameToRenderPass;
	};
}

#endif