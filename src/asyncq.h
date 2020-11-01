#pragma once
#include <thread>
#include <condition_variable>
#include<functional>
#include <deque>
#include <mutex>

template <typename T> class WaitQueue
{//wait without polling until data is stored by push_back -> consume data
public:
	WaitQueue(std::function<void(T)> consumer);
	WaitQueue(WaitQueue&& r);
	void push_back(T s);

private:
	std::deque<T> q;
	std::function<void(T)> consumer;
	std::thread tho;
	std::timed_mutex mtx;
	std::condition_variable_any cv;
	void consume();
};

template <typename T> class AsyncQueue : public WaitQueue<T>
{//for asyncronous connection
public:
	AsyncQueue(std::function<T()> provider, std::function<void(T)> consumer);
	AsyncQueue(AsyncQueue&& r);
	
private:
	std::function<T()> provider;///<auto respond func
	void provide();
	std::thread thi;
};

template<class T, int N> class WQueue
{//for conveyer belt style workfolw
public:
	WQueue(std::function<void(T)> consumer) : th_{&WQueue::run, this, consumer}
	{ }
	void push_back(T s) {
		std::unique_lock<std::mutex> lck{mtx_};
		while(full()) cv_.wait(lck);
		buffer_[end_] = s;
		increase(end_);
		lck.unlock();
		cv_.notify_one();
	}
	~WQueue() { 
		run_ = false;
		th_.join();
	}

protected:
	std::thread th_;
	std::array<T, N> buffer_;
	int start_ = 0, end_ = 0;//valid [start_, end_)
	bool run_ = true;
	std::condition_variable_any cv_;
	std::mutex mtx_;

private:
	bool full() { return end_ == start_ -1 || (start_ == 0 && end_ == N-1); }
	bool empty() { return start_ == end_; }
	void increase(int &i) { i++; if(i == N) i = 0; }
	void run(std::function<void(T)> f) {
		std::unique_lock<std::mutex> lck{mtx_, std::defer_lock};
		while(run_) {
			if(!empty()) {
				f(buffer_[start_]);
				increase(start_);
				cv_.notify_all();
			} else {
				lck.lock();
				while(empty()) cv_.wait(lck);
				lck.unlock();
			}
		}
	}
};

