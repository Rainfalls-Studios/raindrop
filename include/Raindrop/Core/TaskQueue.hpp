#ifndef __RAINDROP_CORE_TASK_QUEUE_HPP__
#define __RAINDROP_CORE_TASK_QUEUE_HPP__

#include <Raindrop/Core/common.hpp>

namespace Raindrop::Core{
	class TaskQueue{
		public:
			using Task = std::function<void(void)>;

			TaskQueue();
			~TaskQueue();

			void add(Task task);
			void run();

		private:
			std::queue<Task> _tasks;
	};
}

#endif