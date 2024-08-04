#include <Raindrop/Graphics/MeshData.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics{
	MeshData::Buffer::Buffer() : 
		data{},
		stride{0}
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
		const auto& bindings = _layout.getBindings();

		std::size_t id = 0;
		for (const auto& binding : bindings){
			const auto& data = binding.second;

			if (data.rate != VK_VERTEX_INPUT_RATE_VERTEX) continue;

			_bindingToBuffer[data.binding] = id;

			Buffer& buffer =_buffers.emplace_back();
			buffer.stride = data.stride;
			buffer.binding = data.binding;

			id++;
		}
	}
			
	void MeshData::allocate(const std::size_t& count){
		_vertexCount = count;
		for (auto& b : _buffers){
			b.data.clear();
			b.data.resize(count * b.stride);
		}
	}

	void MeshData::resize(const std::size_t& count){
		for (auto& b : _buffers){
			b.data.resize(count * b.stride);
		}
	}

	MeshData::Binding MeshData::getBinding(const std::string& name){
		VertexLayout::BindingInfo& info = _layout[name];
		std::size_t index = 0;
		
		try{
			index = _bindingToBuffer.at(info.binding);
		} catch (const std::out_of_range& err){
			_context->logger->warn("Attempt to access instance rate binding in mesh data : The mesh data only contains vertex rate bindings");
			throw std::runtime_error("The mesh data only contains vertex rate bindings : attempt to access an instance rate binding");
		}

		return Binding(info, _buffers[index]);
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

	const std::vector<MeshData::Buffer>& MeshData::getBuffers() const noexcept{
		return _buffers;
	}

	const std::size_t& MeshData::getVertexCount() const noexcept{
		return _vertexCount;
	}

}