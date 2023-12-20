#include <Raindrop/Graphics/Buffers/Loader.hpp>
#include <Raindrop/Graphics/Buffers/Context.hpp>
#include <Raindrop/Graphics/Utils/StringToVulkan.hpp>

namespace Raindrop::Graphics::Buffers{
	Loader::Loader(Context& context) : _context{context}{}
	Loader::~Loader(){}

	void Loader::loadVertexLayouts(const YAML::Node& node){
		_context.logger().info("Loading vertex layouts...");

		for (const auto& layout : node){
			loadVertexLayout(layout);
		}
	}

	void Loader::loadVertexLayout(const YAML::Node& node){
		std::shared_ptr<VertexLayout> layout = std::make_shared<VertexLayout>(_context);

		std::string name;
		try{
			name = node["name"].as<std::string>();
		} catch (const std::exception &e){
			_context.logger().error("Cannot find \"name\" node in vertex layout at line {}", node.Mark().line);
			throw std::runtime_error("Cannot find \"name\" node");
		}

		const auto& bindings = node["bindings"];
		if (bindings){
			std::size_t i = 0;
			for (const auto& binding : bindings){
				std::size_t bindingID = binding["binding"].as<std::size_t>(i);
				VkVertexInputRate inputRate;
				
				try{
					inputRate = binding["inputRate"].as<VkVertexInputRate>();
				} catch (const std::exception &e){
					_context.logger().error("A vertex layout binding MUST have a vertex input rate at line {}", binding.Mark().line);
					throw std::runtime_error("Missing vertex input rate");
				}

				const auto& attributes = binding["attributes"];
				if (attributes){
					for (const auto& attribute : attributes){
						std::string name;
						try{
							name = attribute["name"].as<std::string>();
						} catch (const std::exception &e){
							_context.logger().error("Cannot find vertex layout attribute \"name\" node at line {}", attribute.Mark().line);
							throw std::runtime_error("Missing attribute \"name\" node");
						}

						VkFormat format;
						try{
							format = attribute["format"].as<VkFormat>();
						} catch (const std::exception &e){
							_context.logger().error("Cannot find vertex layout attribute \"format\" node at line {}", attribute.Mark().line);
							throw std::runtime_error("Missing attribute \"format\" node");
						}

						VertexAttributeType type = attribute["type"].as<VertexAttributeType>(VertexAttributeType::None);

						layout->addAttribute(name, bindingID, format, type);
					}
				}

				i++;
			}
		}

		_context.vertexdLayoutRegistry().get(name) = layout;
	}

	static const std::unordered_map<std::string, VertexAttributeType> strToVertexAttributeTypeMap = {
		{"Position", VertexAttributeType::Position},
		{"Normal", VertexAttributeType::Normal},
		{"Uv", VertexAttributeType::Uv},
		{"Color", VertexAttributeType::Color},
		{"Tangent", VertexAttributeType::Tangent},
		{"Bitangent", VertexAttributeType::Bitangent},
		{"BoneWeight", VertexAttributeType::BoneWeight},
		{"BoneIndex", VertexAttributeType::BoneIndex},
		{"Custom", VertexAttributeType::Custom},
		{"None", VertexAttributeType::None}
	};

	std::string Loader::VertexAttributeTypeToStr(const VertexAttributeType& value){
		switch (value){
			case Position: return "Position";
			case Normal: return "Normal";
			case Uv: return "Uv";
			case Color: return "Color";
			case Tangent: return "Tangent";
			case Bitangent: return "Bitangent";
			case BoneWeight: return "BoneWeight";
			case BoneIndex: return "BoneIndex";
			case Custom: return "Custom";
			case None: return "None";
		}
		return "None";
	}

	VertexAttributeType Loader::strToVertexAttributeType(const std::string& str){
		auto it = strToVertexAttributeTypeMap.find(str);
		if (it == strToVertexAttributeTypeMap.end()){
			return VertexAttributeType::None;
		}
		return it->second;
	}
}