#ifndef __RAINDROP_GRAPHICS_PROJECTIONS_ORTHOGRAPHIC_PROJECTION_HPP__
#define __RAINDROP_GRAPHICS_PROJECTIONS_ORTHOGRAPHIC_PROJECTION_HPP__

#include <Raindrop/Graphics/Projections/common.hpp>
#include <Raindrop/Graphics/Projections/Projection.hpp>

namespace Raindrop::Graphics::Projections{
	class OrthographicProjection : public Projection{
		public:
			OrthographicProjection();
			virtual ~OrthographicProjection() override = default;

			void setProjection(float width, float height, float depth);

		private:
			float _width;
			float _height;
			float _depth;


			virtual void update() override;


	};
}

#endif