#ifndef __PROGRAD_MODULE_HPP__
#define __PROGRAD_MODULE_HPP__

#include "common.hpp"

namespace Prograd{
	class Interface : public ::Raindrop::Modules::ModuleInterface{
		public:
			Interface();
			virtual ~Interface() override;

			// virtual const std::unordered_map<std::string, ::Raindrop::RenderSystemCollection>& renderSystemCollections() const override;
	};
}

#endif