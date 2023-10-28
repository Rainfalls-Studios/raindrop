// #include <Raindrop/Graphics/RenderPassesManager.hpp>

// namespace Raindrop::Graphics{
// 	RenderPassesManager::RenderPassesManager(GraphicsContext& context) : _context{context}{}

// 	RenderPassesManager::~RenderPassesManager(){}

// 	std::weak_ptr<RenderPass> RenderPassesManager::get(const std::string& name){
// 		auto it = _nameToRenderPass.find(name);
// 		if (it != _nameToRenderPass.end()){
// 			return it->second;
// 		}
// 		return {};
// 	}

// 	std::weak_ptr<RenderPass> RenderPassesManager::operator[](const std::string& name){
// 		return get(name);
// 	}

// 	void RenderPassesManager::set(const std::string& name, const std::shared_ptr<RenderPass>& renderPass){
// 		_nameToRenderPass[name] = renderPass;
// 	}

// 	void RenderPassesManager::destroy(const std::string& name){
// 		_nameToRenderPass.erase(name);
// 	}

// 	bool RenderPassesManager::has(const std::string& name) const{
// 		return _nameToRenderPass.find(name) != _nameToRenderPass.end();
// 	}

// }