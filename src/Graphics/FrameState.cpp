#include <Raindrop/Graphics/FrameState.hpp>

namespace Raindrop::Graphics{
	FrameState::FrameState(){
		commandBuffer = VK_NULL_HANDLE;
		camera = nullptr;
	}

	FrameState::operator bool() const{
		return commandBuffer != VK_NULL_HANDLE;
	}

	bool FrameState::operator!() const{
		return commandBuffer == VK_NULL_HANDLE;
	}
}