#ifndef __RAINDROP_GRAPHICS_SHADER_MODULE_FACTORY_HPP__
#define __RAINDROP_GRAPHICS_SHADER_MODULE_FACTORY_HPP__

#include "types.hpp"
#include "pch.pch"
#include <Raindrop/Assets/Factory.hpp>
#include "ShaderModule.hpp"

namespace Raindrop::Graphics{
	class ShaderModuleFactory : public Assets::Factory{
		public:
			ShaderModuleFactory(Context& context);
			~ShaderModuleFactory();

			virtual std::shared_ptr<Assets::Asset> getOrCreate(const std::filesystem::path& path) override;

		private:
			Context* _context;
			std::unordered_map<std::string, std::shared_ptr<ShaderModule>> _modules;

			std::string loadSPVCode(const std::filesystem::path& path);
	};
}

#endif