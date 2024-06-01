#include <Raindrop_internal/Operators.hpp>

namespace Raindrop{
	VkLogicOp toVulkan(const LogicOperator& op){
		switch (op){
			case LogicOperator::CLEAR: return VK_LOGIC_OP_CLEAR;
			case LogicOperator::AND: return VK_LOGIC_OP_AND;
			case LogicOperator::AND_REVERSE: return VK_LOGIC_OP_AND_REVERSE;
			case LogicOperator::COPY: return VK_LOGIC_OP_COPY;
			case LogicOperator::AND_INVERTED: return VK_LOGIC_OP_AND_INVERTED;
			case LogicOperator::NO_OP: return VK_LOGIC_OP_NO_OP;
			case LogicOperator::XOR: return VK_LOGIC_OP_XOR;
			case LogicOperator::OR: return VK_LOGIC_OP_OR;
			case LogicOperator::NOR: return VK_LOGIC_OP_NOR;
			case LogicOperator::EQUIVALENT: return VK_LOGIC_OP_EQUIVALENT;
			case LogicOperator::INVERT: return VK_LOGIC_OP_INVERT;
			case LogicOperator::OR_REVERSE: return VK_LOGIC_OP_OR_REVERSE;
			case LogicOperator::COPY_INVERTED: return VK_LOGIC_OP_COPY_INVERTED;
			case LogicOperator::OR_INVERTED: return VK_LOGIC_OP_OR_INVERTED;
			case LogicOperator::NAND: return VK_LOGIC_OP_NAND;
			case LogicOperator::SET: return VK_LOGIC_OP_SET;
		};
		throw std::runtime_error("Invalid logic operation");
	}

	VkCompareOp toVulkan(const CompareOperator& op){
		switch (op){
			case CompareOperator::NEVER: return VK_COMPARE_OP_NEVER;
			case CompareOperator::LESS: return VK_COMPARE_OP_LESS;
			case CompareOperator::EQUAL: return VK_COMPARE_OP_EQUAL;
			case CompareOperator::LESS_OR_EQUAL: return VK_COMPARE_OP_LESS_OR_EQUAL;
			case CompareOperator::GREATER: return VK_COMPARE_OP_GREATER;
			case CompareOperator::NOT_EQUAL: return VK_COMPARE_OP_NOT_EQUAL;
			case CompareOperator::GREATER_OR_EQUAL: return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case CompareOperator::ALWAYS: return VK_COMPARE_OP_ALWAYS;
		}
		throw std::runtime_error("Invalid compare operation");
	}
}