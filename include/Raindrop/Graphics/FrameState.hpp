#ifndef __RAINDROP_GRAPHICS_FRAME_STATE_HPP__
#define __RAINDROP_GRAPHICS_FRAME_STATE_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	struct FrameState{
		VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
	};
}

#endif