#ifndef __RAINDROP_GRAPHICS_SHADERS_SHADER_FACTORY_HPP__
#define __RAINDROP_GRAPHICS_SHADERS_SHADER_FACTORY_HPP__

#include <Raindrop/Graphics/Shaders/common.hpp>
#include <Raindrop/Core/Asset/AssetFactory.hpp>

namespace Raindrop::Graphics::Shaders{
	class ShaderFactory : public Core::Asset::AssetFactory{
		public:
			ShaderFactory(Context& context);
			virtual ~ShaderFactory() override;

			virtual std::shared_ptr<Core::Asset::Asset> createAsset(const std::filesystem::path& path) override;
			virtual void destroyAsset(std::shared_ptr<Core::Asset::Asset> asset) override;

			virtual std::vector<const char*> extensions() const override;
		
		protected:
			virtual bool has(const std::shared_ptr<Core::Asset::Asset>& asset) const override;

		private:
			Context& _context;
			std::list<std::shared_ptr<Shader>> _shaders;

	};
}

#endif