#include <Raindrop/Core/TaskQueue.hpp>

namespace Raindrop::Core{
	TaskQueue::TaskQueue(){}
	TaskQueue::~TaskQueue(){}

	void TaskQueue::add(Task task){
		_tasks.push(task);
	}
	
	void TaskQueue::run(){
		while (!_tasks.empty()){
			auto task = _tasks.front();	
			task();
			_tasks.pop();
		}
	}
}