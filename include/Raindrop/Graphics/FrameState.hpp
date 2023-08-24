#ifndef __RAINDROP_GRAPHICS_FRAME_STATE_HPP__
#define __RAINDROP_GRAPHICS_FRAME_STATE_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	struct FrameState{
		FrameState();
		
		VkCommandBuffer commandBuffer;
		Camera* camera;

		operator bool() const;
		bool operator!() const;
	};
}

#endif