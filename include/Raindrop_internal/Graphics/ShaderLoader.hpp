#ifndef __RAINDROP_INTERNAL_GRAPHICS_SHADER_LOADER_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_SHADER_LOADER_HPP__

#include "common.hpp"
#include <Raindrop/Asset.hpp>

namespace Raindrop::Internal::Graphics{
	class ShaderLoader : public Asset::Loader{
		public:
			ShaderLoader(Context& context);
			
			virtual std::shared_ptr<Asset> load(const Path& path) override;

		private:
			Context& _context;
	};
}

#endif