#ifndef __RAINDROP_GRAPHICS_CAMERA_HPP__
#define __RAINDROP_GRAPHICS_CAMERA_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/SceneFramebuffer.hpp>
#include <Raindrop/Graphics/Projections/common.hpp>

namespace Raindrop::Graphics{
	class Camera{
		public:
			Camera(GraphicsContext& context, uint32_t width, uint32_t height);
			~Camera();

			const Target& framebuffer() const;

			Projections::OrthographicProjection& orthographic();
			Projections::PerspectiveProjection& perspective();
			Projections::ProjectionType projectionType() const;

		protected:

			GraphicsContext& _context;
			SceneFramebuffer _framebuffer;

			std::unique_ptr<Projections::Projection> _projection;
			Projections::ProjectionType _projectionType;
			
			void setProjection(Projections::ProjectionType type);
			void checkProjection(Projections::ProjectionType type);
	};
}

#endif