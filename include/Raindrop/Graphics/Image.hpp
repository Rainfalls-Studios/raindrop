#ifndef __RAINDROP_GRAPHICS_IMAGE_HPP__
#define __RAINDROP_GRAPHICS_IMAGE_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Graphics{
	class Image{
		public:

		private:
			Context* _context;
			VkImage _image;
	};

}

#endif