#ifndef __RAINDROP_GRAPHICS_COMPONENTS_CAMERA_HPP__
#define __RAINDROP_GRAPHICS_COMPONENTS_CAMERA_HPP__

#include <Raindrop/Graphics/Components/common.hpp>
#include <Raindrop/Core/Scene/Components/common.hpp>
#include <Raindrop/Graphics/Camera.hpp>

namespace Raindrop::Graphics::Components{
	class Camera : public Component{
		public:
			Camera(GraphicsContext& context);
			virtual ~Camera() override;

			const Graphics::Camera& camera() const;
			Graphics::Camera& camera();
		
		private:
			Graphics::Camera _camera;

	};
}

#endif