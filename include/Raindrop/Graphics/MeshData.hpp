#ifndef __RAINDROP_GRAPHICS_MESH_DATA_HPP__
#define __RAINDROP_GRAPHICS_MESH_DATA_HPP__

#include "types.hpp"
#include "pch.pch"
#include "VertexLayout.hpp"

namespace Raindrop::Graphics{
	class MeshData{
		public:
			struct Buffer{
				std::vector<uint8_t> data;
				std::uint32_t stride;
				std::uint32_t binding;

				Buffer();
			};

			struct AttributeData{
				std::uint32_t binding;
				std::uint32_t offset;
			};

			class Binding{
				public:
					Binding(VertexLayout::BindingInfo& info, Buffer& buffer) noexcept;
					~Binding() = default;

					template<typename T>
					class Attribute{
						public:
							Attribute(const std::size_t& offset, Buffer& buffer) noexcept :
								_offset{offset},
								_buffer{buffer}
							{}

							~Attribute() = default;

							T& operator[](const std::size_t& index){
								std::size_t id = _buffer.stride * index + _offset;
								return reinterpret_cast<T&>(_buffer.data.at(id));
							}

							const T& operator[](const std::size_t& index) const{
								std::size_t id = _buffer.stride * index + _offset;
								return reinterpret_cast<T&>(_buffer.data.at(id));
							}

						private:
							std::size_t _offset;
							Buffer& _buffer;
					};

					template<typename T>
					inline Attribute<T> get(const std::string& name){
						return Attribute<T>(_info[name].offset, _buffer);
					}

					void* viewRaw();

					template<typename T>
					inline T* view(){
						return static_cast<T*>(viewRaw());
					}


				private:
					VertexLayout::BindingInfo& _info;
					Buffer& _buffer;
			};

			MeshData() noexcept;
			~MeshData();

			MeshData(MeshData&& other);
			MeshData& operator=(MeshData&& other);

			friend void swap(MeshData& A, MeshData& B);

			void prepare(Context& context);
			void setLayout(const VertexLayout& layout);
			
			void allocate(const std::size_t& count);
			void resize(const std::size_t& count);
			void pushIndex(const std::size_t& index);

			Binding getBinding(const std::string& name);

			const std::vector<std::size_t>& getIndices() const noexcept;
			std::vector<std::uint8_t> getIndicesAs(const VkIndexType& type) const noexcept;
			const std::vector<std::uint8_t>& getBindingBuffer(const std::string& name) const;
			const VertexLayout& getLayout() const noexcept;

			const std::vector<Buffer>& getBuffers() const noexcept;
			const std::size_t& getVertexCount() const noexcept;


		private:
			Context* _context;
			std::size_t _vertexCount;
			std::vector<Buffer> _buffers;
			std::vector<std::size_t> _indices;
			std::unordered_map<std::size_t, std::size_t> _bindingToBuffer;
			VertexLayout _layout;
	};
}

#endif