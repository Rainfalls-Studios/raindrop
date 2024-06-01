#ifndef __RAINDROP_INTERNAL_OPERATORS_HPP__
#define __RAINDROP_INTERNAL_OPERATORS_HPP__

#include <Raindrop/Operators.hpp>
#include <Raindrop_internal/Context.hpp>

namespace Raindrop{
	VkLogicOp toVulkan(const LogicOperator& op);
	VkCompareOp toVulkan(const CompareOperator& op);
}

#endif