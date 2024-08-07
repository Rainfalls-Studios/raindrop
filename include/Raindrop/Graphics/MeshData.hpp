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
				VkMemoryPropertyFlags memProperties;

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

							void* getRaw(const std::size_t& index){
								std::size_t id = _buffer.stride * index + _offset;
								return static_cast<void*>(_buffer.data.data() + id);
							}

						private:
							std::size_t _offset;
							Buffer& _buffer;
					};

					template<typename T = std::nullptr_t>
					inline Attribute<T> get(const std::string& name){
						return Attribute<T>(_info[name].offset, _buffer);
					}

					template<typename T = std::nullptr_t>
					inline Attribute<T> get(const std::size_t& location){
						for (const auto& attribute : _info.attributes){
							if (attribute.second.location == location){
								return Attribute<T>(attribute.second.offset, _buffer);
							}
						}
						throw std::out_of_range("Invalid location");
					}

					void* viewRaw();

					template<typename T>
					inline T* view(){
						return static_cast<T*>(viewRaw());
					}


				private:
					VertexLayout::BindingInfo& _info;
					Buffer& _buffer;

					const VertexLayout::AttributeInfo* get(const std::uint32_t& location) const noexcept;
			};

			MeshData() noexcept;
			~MeshData();

			MeshData(MeshData&& other);
			MeshData& operator=(MeshData&& other);

			friend void swap(MeshData& A, MeshData& B);

			MeshData& prepare(Context& context);
			MeshData& setLayout(const VertexLayout& layout);
			
			MeshData& allocate(const std::size_t& count);
			void resize(const std::size_t& count);
			void pushIndex(const std::size_t& index);
			MeshData& setPrimitiveTopology(const VkPrimitiveTopology& topology);

			Binding getBinding(const std::string& name);
			Binding getBinding(const std::uint32_t& binding);

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
			VkPrimitiveTopology _primitiveTopology;
	};
}

#endif