#ifndef __RAINDROP_GRAPHICS_CAMERA_HPP__
#define __RAINDROP_GRAPHICS_CAMERA_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/Projections/common.hpp>

namespace Raindrop::Graphics{
	class Camera{
		public:
			Camera(Context& context, std::size_t width, std::size_t height);
			~Camera();
			
			Projections::OrthographicProjection& orthographic();
			Projections::PerspectiveProjection& perspective();
			Projections::ProjectionType projectionType() const;

			const glm::mat4& viewProjection() const;
			const glm::mat4& view() const;
			const glm::mat4& projection() const;

		protected:
			Context& _context;

			std::unique_ptr<Projections::Projection> _projection;
			Projections::ProjectionType _projectionType;
			
			void setProjection(Projections::ProjectionType type);
			void checkProjection(Projections::ProjectionType type);
	};
}

#endif