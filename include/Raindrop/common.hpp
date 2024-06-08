#ifndef __RAINDROP_COMMON_HPP__
#define __RAINDROP_COMMON_HPP__

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <typeindex>
#include <list>
#include <string>
#include <functional>
#include <memory>
#include <filesystem>
#include <optional>

namespace Raindrop{
	using Path = std::filesystem::path;

	class Context;
	class Format;
	class Texture;
	class RenderPass;
	class GUID;
	class TextureSubset;
	class Color;
	class Event;
	class Asset;
	class Pipeline;
	class CommandBuffer;
	class Buffer;
}

#endif