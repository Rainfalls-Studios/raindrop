#include <Raindrop/Graphics/MeshData.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics{
	MeshData::Buffer::Buffer() : 
		data{},
		stride{0},
		isVertex{false},
		valid{false}
	{}

	MeshData::Binding::Binding(VertexLayout::BindingInfo& info, Buffer& buffer) noexcept :
		_info{info},
		_buffer{buffer}
	{}

	void* MeshData::Binding::viewRaw(){
		return _buffer.data.data();
	}

	MeshData::MeshData() noexcept :
		_context{nullptr},
		_buffers{},
		_indices{},
		_layout{}
	{}

	MeshData::~MeshData(){}

	MeshData::MeshData(MeshData&& other) : 
		_context{nullptr},
		_buffers{},
		_indices{},
		_layout{}
	{
		swap(*this, other);
	}

	MeshData& MeshData::operator=(MeshData&& other){
		swap(*this, other);
		return *this;
	}

	void swap(MeshData& A, MeshData& B){
		std::swap(A._context, B._context);
		std::swap(A._buffers, B._buffers);
		std::swap(A._indices, B._indices);
		std::swap(A._layout, B._layout);
	}

	void MeshData::prepare(Context& context){
		_context = &context;
	}
	
	void MeshData::setLayout(const VertexLayout& layout){
		_layout = layout;

		_buffers.clear();
		_buffers.resize(_layout.getBindingCount());

		auto descriptions = _layout.getBindingDescriptions();
		for (auto& d : descriptions){
			auto& buffer = _buffers[d.binding];

			buffer.isVertex = d.inputRate == VK_VERTEX_INPUT_RATE_VERTEX;
			buffer.stride = d.stride;
			buffer.valid = true;
		}
	}
			
	void MeshData::allocate(const std::size_t& count){
		for (auto& b : _buffers){
			if (!b.isVertex || !b.valid) continue;

			b.data.clear();
			b.data.resize(count * b.stride);
		}
	}

	void MeshData::resize(const std::size_t& count){
		for (auto& b : _buffers){
			if (!b.isVertex || !b.valid) continue;

			b.data.resize(count * b.stride);
		}
	}

	MeshData::Binding MeshData::getBinding(const std::string& name){
		VertexLayout::BindingInfo& info = _layout[name];
		return Binding(info, _buffers[info.binding]);
	}

	void MeshData::pushIndex(const std::size_t& index){
		_indices.push_back(index);
	}


	const std::vector<std::size_t>& MeshData::getIndices() const noexcept{
		return _indices;
	}

	const std::vector<std::uint8_t>& MeshData::getBindingBuffer(const std::string& name) const{
		return _buffers[_layout[name].binding].data;
	}

	const VertexLayout& MeshData::getLayout() const noexcept{
		return _layout;
	}

	std::vector<std::uint8_t> MeshData::getIndicesAs(const VkIndexType& type) const noexcept{
		std::vector<std::uint8_t> output;

		switch (type){
			case VK_INDEX_TYPE_UINT8_EXT:{
				output.reserve(_indices.size());
				std::transform(
					_indices.begin(),
					_indices.end(),
					std::back_inserter(output),
					[](const std::size_t& index) -> std::uint8_t {
						return static_cast<std::uint8_t>(index);
					}
				);
				break;
			}

			case VK_INDEX_TYPE_UINT16:{
				output.reserve(_indices.size() * 2);
				
				for (auto& index : _indices){
					output.push_back(static_cast<std::uint8_t>(index & 0xFF));
					output.push_back(static_cast<std::uint8_t>((index >> 8) & 0xFF));
				}
				break;
			}

			case VK_INDEX_TYPE_UINT32:{
				output.reserve(_indices.size() * 4);
				
				for (auto& index : _indices){
					output.push_back(static_cast<uint8_t>(index & 0xFF));
					output.push_back(static_cast<uint8_t>((index >> 8) & 0xFF));
					output.push_back(static_cast<uint8_t>((index >> 16) & 0xFF));
					output.push_back(static_cast<uint8_t>((index >> 24) & 0xFF));
				}
				break;
			}
		}

		return std::move(output);
	}
}