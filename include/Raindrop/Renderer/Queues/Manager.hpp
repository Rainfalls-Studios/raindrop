#ifndef __RAINDROP_RENDERER_QUEUES_MANAGER_HPP__
#define __RAINDROP_RENDERER_QUEUES_MANAGER_HPP__

#include "common.hpp"
#include "../Core/structs.hpp"

namespace Raindrop::Renderer::Queues{
	class Manager{
		public:
			Manager(Context& context);
			~Manager();

			VkQueue graphicsQueue() const;
			VkQueue presentQueue() const;
			VkQueue transfertQueue() const;

			std::size_t graphicsFamily() const;
			std::size_t presentFamily() const;
			std::size_t transfertFamily() const;

		private:
			Context& _context;

			std::size_t _graphicsFamily;
			std::size_t _presentFamily;
			std::size_t _transfertFamily;

			VkQueue _graphicsQueue;
			VkQueue _presentQueue;
			VkQueue _transfertQueue;

			void findGraphicsFamily(std::vector<VkQueueFamilyProperties>& families);
			void findPresentFamily(std::vector<VkQueueFamilyProperties>& families);
			void findTransfertFamily(std::vector<VkQueueFamilyProperties>& families); // has to be executed after finding graphics family

			void getQueues(std::vector<VkQueueFamilyProperties>& families);
	};
}

#endif