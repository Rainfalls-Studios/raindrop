#include <Raindrop/Graphics/Buffers/VertexLayoutRegistry.hpp>
#include <Raindrop/Graphics/Buffers/Context.hpp>

namespace Raindrop::Graphics::Buffers{
	VertexLayoutRegistry::VertexLayoutRegistry(Context& context) : _context{context}{}
	VertexLayoutRegistry::~VertexLayoutRegistry(){}

	const std::shared_ptr<VertexLayout> VertexLayoutRegistry::get(const std::string& name) const{
		auto it = _layouts.find(name);
		if (it == _layouts.end()){
			_context.logger().trace("Failed to find \"{}\" vertex layout", name);
			return nullptr;
		}
		return it->second;
	}

	std::shared_ptr<VertexLayout>& VertexLayoutRegistry::get(const std::string& name){
		return _layouts[name];
	}

	void VertexLayoutRegistry::remove(const std::string& name){
		_layouts.erase(name);
	}
}