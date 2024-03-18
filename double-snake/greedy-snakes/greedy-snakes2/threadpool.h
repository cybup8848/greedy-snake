#pragma once
#include<vector>
#include<queue>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<functional>
#include<future>
#include<type_traits>
class ThreadPool {
public:
	ThreadPool(int num);

	template<typename F, typename ...Args>
	//auto enqueue(F&& f, Args... args) -> std::future<std::invoke_result_t<F,Args...>::type> ;
	auto enqueue(F&& f, Args... args) -> std::future<typename std::result_of<F(Args...)>::type>;

	~ThreadPool();

private:
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;
	std::mutex mutex;
	std::condition_variable condition;
	bool stop;
};

template<typename F, typename ...Args>
auto ThreadPool::enqueue(F&& f, Args... args) -> std::future<typename std::result_of<F(Args...)>::type> {
	using return_type = typename std::result_of<F(Args...)>::type;
	auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
	std::future<return_type> future = task->get_future();
	{
		std::unique_lock<std::mutex> lock(this->mutex);
		if (stop) {
			throw std::runtime_error("enqueue on stopped ThreadPool");
		}
		this->tasks.emplace([task]() {(*task)();});//优先级：*task()错误      (*task)()正确
	}
	this->condition.notify_one();
	return future;
}


