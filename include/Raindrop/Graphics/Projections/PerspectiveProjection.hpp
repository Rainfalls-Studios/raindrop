#ifndef __RAINDROP_GRAPHICS_PROJECTIONS_PERSPECTIVE_PROJECTION_HPP__
#define __RAINDROP_GRAPHICS_PROJECTIONS_PERSPECTIVE_PROJECTION_HPP__

#include <Raindrop/Graphics/Projections/common.hpp>
#include <Raindrop/Graphics/Projections/Projection.hpp>

namespace Raindrop::Graphics::Projections{
	class PerspectiveProjection : public Projection{
		public:
			PerspectiveProjection();
			virtual ~PerspectiveProjection() override = default;

			void setProjection(float fov, float nearClip, float farClip, float aspectRatio);

		private:
			float _fov;
			float _nearClip;
			float _farClip;
			float _aspectRatio;

			virtual void update() override;
	};
}

#endif