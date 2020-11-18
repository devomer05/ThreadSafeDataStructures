#ifndef _ThreadSafeArray__
#define _ThreadSafeArray__

#include <mutex>

constexpr int max_size = 32;
constexpr int min_size = 4;

template <typename T>
class ThreadSafeArray
{
protected:
	T* arr;
	int capacity;
	std::mutex m;
public:
	virtual bool Push(const T& val) = 0;
	virtual bool Pop(T& valOut) = 0;
	virtual bool IsFull() = 0;
	virtual bool IsEmpty() = 0;
	virtual int Size() = 0;
	virtual void Print() = 0;
};

#endif;
