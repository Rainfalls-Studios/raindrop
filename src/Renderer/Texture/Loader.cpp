#include <Raindrop/Graphics/Textures/Loader.hpp>
#include <Raindrop/Graphics/Textures/Texture.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Textures{
	Loader::Loader(Context &context) : _context{context}{
		spdlog::info("Constructing texture asset loader ...");
	}

	Loader::~Loader(){
		spdlog::info("Destroying texture asset loader ...");
	}

	std::shared_ptr<Loader::Asset> Loader::load(const Path& path){
		spdlog::info("Loading texture from \"{}\"", path.string());

		std::shared_ptr<Texture> texture;
		try{
			texture = std::make_shared<Texture>(_context, path);
		} catch (const std::exception& e){
			spdlog::error("Failed to load texture from \"{}\" :: reason : \"{}\"", path.string(), e.what());
			throw std::runtime_error("Failed to load texture");
		}
		_textures.push_back(texture);

		return std::static_pointer_cast<Loader::Asset>(texture);
	}

	void Loader::registerAsset(const Path& path, const std::shared_ptr<Asset>& asset){
		_textures.push_back(std::static_pointer_cast<Texture>(asset));
	}

	void Loader::collectGarbage(){
		auto it = _textures.begin();
		std::size_t count = 0;
		while (it != _textures.end()){
			if (it->unique()){
				count++;
				_textures.erase(it);
				continue;
			}
			it++;
		}

		spdlog::trace("Cleared {} etxture assets from texture loader garbage collection", count);
	}
}