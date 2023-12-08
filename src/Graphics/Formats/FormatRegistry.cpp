#include <Raindrop/Graphics/Formats/FormatRegistry.hpp>
#include <Raindrop/Graphics/Formats/Context.hpp>

namespace Raindrop::Graphics::Formats{
	FormatRegistry::FormatRegistry(Context& context) : _context{context}{}
	FormatRegistry::~FormatRegistry(){}


	void FormatRegistry::registerFormat(VkFormat format, const std::string& name){
		_formats[name] = format;
	}

	VkFormat FormatRegistry::get(const std::string& name) const{
		auto it = _formats.find(name);
		if (it == _formats.end()) return VK_FORMAT_UNDEFINED;
		return it->second;
	}
}