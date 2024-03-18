#include"ThreadPool.h"
ThreadPool::ThreadPool(int num):stop(false) {
	for (int i = 0;i < num;i++) {
		this->workers.emplace_back([this]() {
			for (;;) {
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(this->mutex);
					this->condition.wait(lock, [this]{return this->stop || !this->tasks.empty();});
					if (this->stop && this->tasks.empty()) { 
						return; 
					}
					task = std::move(this->tasks.front());
					this->tasks.pop();
				}
				task();
			}
		});
	}
}

ThreadPool::~ThreadPool() {
	{
		std::unique_lock<std::mutex> lock(this->mutex);
		this->stop = true;
	}
	this->condition.notify_all();
	for (auto& worker : this->workers) {
		worker.join();
	}
	
}



