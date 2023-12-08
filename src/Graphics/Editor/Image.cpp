// #include <Raindrop/Graphics/Editor/Image.hpp>
// #include <imgui/backends/imgui_impl_vulkan.h>

// namespace Raindrop::Graphics::Editor{
// 	Image::Image(EditorContext& context) : _context{context}{

// 	}

// 	Image::~Image(){
// 	}

// 	void Image::setTexture(VkSampler sampler, VkImageView imageView, VkImageLayout layout){
// 		removeTexture();
// 		_descriptorSet = ImGui_ImplVulkan_AddTexture(sampler, imageView, layout);
// 	}

// 	void Image::removeTexture(){
// 		if (_descriptorSet != VK_NULL_HANDLE) ImGui_ImplVulkan_RemoveTexture(_descriptorSet);
// 		_descriptorSet = VK_NULL_HANDLE;
// 	}

// 	ImTextureID Image::texture() const{
// 		return static_cast<ImTextureID>(_descriptorSet);
// 	}

// }