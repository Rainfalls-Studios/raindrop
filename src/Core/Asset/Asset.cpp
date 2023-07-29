#include <Raindrop/Core/Asset/Asset.hpp>

namespace Raindrop::Core::Asset{
	const std::filesystem::path& Asset::path() const{
		return _path;
	}
}