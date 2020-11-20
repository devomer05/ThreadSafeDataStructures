#ifndef _ThreadSafeStack__
#define _ThreadSafeStack__

#include "ThreadSafeArray.h"

#include <algorithm>

template <class T>
class ThreadSafeArray;

template <class T>
class ThreadSafeStack : public ThreadSafeArray<T>
{
private:	
	int top;
	bool _IsFull()
	{
		return (top == this->capacity - 1);
	}

	bool _IsEmpty()
	{
		return (top == -1);
	}

	int _Size()
	{
		return top + 1;
	}

public:
	bool Push(const T& val)
	{
		std::lock_guard<std::mutex> guard( this->m );
		if (_IsFull())
		{
			int newCapacity = std::min(this->capacity * 3 / 2 + 1, max_size);
			if (newCapacity > this->capacity)
			{
				this->capacity = newCapacity;
				T* newArr = new T[this->capacity];
				memcpy(newArr, this->arr, _Size() * sizeof(T));
				delete[] this->arr;
				this->arr = newArr;
			}
			else
			{
				return false;
			}
		}
		this->arr[++top] = val;
		return true;
	}

	bool Pop(T& valOut)
	{
		std::lock_guard<std::mutex> guard(this->m);
		if (_IsEmpty())
		{
			return false;
		}
		else
		{
			valOut = this->arr[top--];

			//std::cout << "REMOVED VALUE: " << valOut << std::endl;

			if (top < (this->capacity / 3) - 1)
			{
				int newCapacity = std::max(this->capacity / 2, min_size);
				if (newCapacity < this->capacity)
				{
					this->capacity = newCapacity;
					T* newArr = new T[this->capacity];



					delete[] this->arr;
					this->arr = newArr;
				}
			}
		}
		return true;
	}

	bool IsFull()
	{
		std::lock_guard<std::mutex> guard(this->m);
		return (top == this->capacity - 1);
	}

	bool IsEmpty()
	{
		std::lock_guard<std::mutex> guard(this->m);
		return (top == -1);
	}

	int Size()
	{
		std::lock_guard<std::mutex> guard(this->m);
		return top + 1;
	}

	void Print()
	{
		std::lock_guard<std::mutex> guard(this->m);
		std::cout << std::endl;

		if (_Size() == 0)
			std::cout << "!!! Stack is empty !!!";
		else
		{
			std::cout << "Content: ";
			for (int i = 0; i < _Size(); i++)
			{
				std::cout << this->arr[i] << " ";
			}
		}
		std::cout << "  ( Size: " << _Size() << " Capacity: " << this->capacity << " )";
		std::cout << std::endl;
	}

	ThreadSafeStack(int cap) : ThreadSafeArray<T>(cap)
	{
		top = -1;
	}
};

#endif // _ThreadSafeStack__
