#pragma once

#include "Raindrop/pch.pch"

namespace Raindrop::Core::Assets{
	class Asset;

	class Factory{
		public:
			virtual ~Factory() = default;
			
			virtual std::shared_ptr<Asset> getOrCreate(const std::filesystem::path& path) = 0;
	};
}