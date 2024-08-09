#ifndef __RAINDROP_GRAPHICS_MODEL_HPP__
#define __RAINDROP_GRAPHICS_MODEL_HPP__

#include "types.hpp"
#include "pch.pch"
#include <Raindrop/Object.hpp>

namespace Raindrop::Graphics{
	class Model : public Object{
		public:
			Model() noexcept;
			~Model();

			Model(const Model&) = delete;
			Model& operator=(const Model&) = delete;

			Model(Model&& other);
			Model& operator=(Model&& other);

			friend void swap(Model& A, Model& B);

			Model& prepare(Context& context);
			void initialize();
			void release();

			Model& setMeshes(std::vector<Mesh>&& meshes);
			
			const std::vector<Mesh>& getMeshes() const noexcept;

		private:
			Context* _context;

			std::vector<Mesh> _meshes;
	};
}

#endif