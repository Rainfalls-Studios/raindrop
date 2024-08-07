#include <Raindrop/Graphics/Utils/type_to_format.hpp>

namespace Raindrop::Graphics::Utils{
	template<>
	VkFormat typeToFormat<int8_t>() noexcept{
		return VK_FORMAT_R8_SINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<2, int8_t>>() noexcept{
		return VK_FORMAT_R8G8_SINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<3, int8_t>>() noexcept{
		return VK_FORMAT_R8G8B8_SINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<4, int8_t>>() noexcept{
		return VK_FORMAT_R8G8B8A8_SINT;
	}

	template<>
	VkFormat typeToFormat<uint8_t>() noexcept{
		return VK_FORMAT_R8_UINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<2, uint8_t>>() noexcept{
		return VK_FORMAT_R8G8_UINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<3, uint8_t>>() noexcept{
		return VK_FORMAT_R8G8B8_UINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<4, uint8_t>>() noexcept{
		return VK_FORMAT_R8G8B8A8_UINT;
	}

	template<>
	VkFormat typeToFormat<int16_t>() noexcept{
		return VK_FORMAT_R16_SINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<2, int16_t>>() noexcept{
		return VK_FORMAT_R16G16_SINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<3, int16_t>>() noexcept{
		return VK_FORMAT_R16G16B16_SINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<4, int16_t>>() noexcept{
		return VK_FORMAT_R16G16B16A16_SINT;
	}

	template<>
	VkFormat typeToFormat<uint16_t>() noexcept{
		return VK_FORMAT_R16_UINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<2, uint16_t>>() noexcept{
		return VK_FORMAT_R16G16_UINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<3, uint16_t>>() noexcept{
		return VK_FORMAT_R16G16B16_UINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<4, uint16_t>>() noexcept{
		return VK_FORMAT_R16G16B16A16_UINT;
	}

	template<>
	VkFormat typeToFormat<int32_t>() noexcept{
		return VK_FORMAT_R32_SINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<2, int32_t>>() noexcept{
		return VK_FORMAT_R32G32_SINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<3, int32_t>>() noexcept{
		return VK_FORMAT_R32G32B32_SINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<4, int32_t>>() noexcept{
		return VK_FORMAT_R32G32B32A32_SINT;
	}

	template<>
	VkFormat typeToFormat<uint32_t>() noexcept{
		return VK_FORMAT_R32_UINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<2, uint32_t>>() noexcept{
		return VK_FORMAT_R32G32_UINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<3, uint32_t>>() noexcept{
		return VK_FORMAT_R32G32B32_UINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<4, uint32_t>>() noexcept{
		return VK_FORMAT_R32G32B32A32_UINT;
	}

	template<>
	VkFormat typeToFormat<int64_t>() noexcept{
		return VK_FORMAT_R64_SINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<2, int64_t>>() noexcept{
		return VK_FORMAT_R64G64_SINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<3, int64_t>>() noexcept{
		return VK_FORMAT_R64G64B64_SINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<4, int64_t>>() noexcept{
		return VK_FORMAT_R64G64B64A64_SINT;
	}

	template<>
	VkFormat typeToFormat<uint64_t>() noexcept{
		return VK_FORMAT_R64_UINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<2, uint64_t>>() noexcept{
		return VK_FORMAT_R64G64_UINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<3, uint64_t>>() noexcept{
		return VK_FORMAT_R64G64B64_UINT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<4, uint64_t>>() noexcept{
		return VK_FORMAT_R64G64B64A64_UINT;
	}

	template<>
	VkFormat typeToFormat<float>() noexcept{
		return VK_FORMAT_R32_SFLOAT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<2, float>>() noexcept{
		return VK_FORMAT_R32G32_SFLOAT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<3, float>>() noexcept{
		return VK_FORMAT_R32G32B32_SFLOAT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<4, float>>() noexcept{
		return VK_FORMAT_R32G32B32A32_SFLOAT;
	}

	template<>
	VkFormat typeToFormat<double>() noexcept{
		return VK_FORMAT_R64_SFLOAT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<2, double>>() noexcept{
		return VK_FORMAT_R64G64_SFLOAT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<3, double>>() noexcept{
		return VK_FORMAT_R64G64B64_SFLOAT;
	}

	template<>
	VkFormat typeToFormat<glm::vec<4, double>>() noexcept{
		return VK_FORMAT_R64G64B64A64_SFLOAT;
	}
}