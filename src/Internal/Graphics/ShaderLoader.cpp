#include <Raindrop_internal/Graphics/ShaderLoader.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Graphics/Shader.hpp>
#include <Raindrop/Pipeline.hpp>

namespace Raindrop::Internal::Graphics{
	ShaderLoader::ShaderLoader(Context& context) : _context{context}{}
			
	std::shared_ptr<Asset> ShaderLoader::load(const Path& path){
		_context.getLogger()->info("Loading a shader ... (\"{}\")", path.string());
		return std::make_shared<Pipeline::Shader>(_context.getInternalContext().getInterface(), path);
	}
}