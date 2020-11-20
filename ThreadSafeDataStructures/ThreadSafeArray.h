#ifndef _ThreadSafeArray__
#define _ThreadSafeArray__
#include <mutex>

constexpr int max_size = 64;
constexpr int min_size = 4;

template <typename T>
class ThreadSafeArray
{
protected:
	T* arr;
	int capacity;
	int total_count;
	std::mutex m;
public:
	ThreadSafeArray(int cap)
	{
		this->capacity = std::max(min_size, std::min(cap, max_size));
		this->arr = new T[this->capacity];
		total_count = 0;
	}

	~ThreadSafeArray()
	{
		delete[] this->arr;
	}
	virtual bool Push(const T& val) = 0;
	virtual bool Pop(T& valOut) = 0;
	virtual bool IsFull() = 0;
	virtual bool IsEmpty() = 0;
	virtual int Size() = 0;
	virtual void Print() = 0;
};

#endif;
