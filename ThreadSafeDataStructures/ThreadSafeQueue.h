#ifndef _ThreadSafeQueue__
#define _ThreadSafeQueue__

#include "ThreadSafeArray.h"

template <class T>
class ThreadSafeQueue : public ThreadSafeArray<T>
{
private:
	int rear,front;
	int count;

	bool _IsFull()
	{
		return (count == this->capacity);
	}

	bool _IsEmpty()
	{
		return (count == 0);
	}

	int _Size()
	{
		return count;
	}

	void _Print()
	{
		std::cout << std::endl;

		if (_Size() == 0)
			std::cout << "!!! Queue is empty !!!";
		else
		{
			std::cout << "Content: ";
			if (rear >= front)
			{
				for (int i = front; i <= rear; i++)
				{
					std::cout << this->arr[i] << " ";
				}
			}
			else
			{
				for (int i = front; i < this->capacity; i++)
				{
					std::cout << this->arr[i] << " ";
				}
				for (int i = 0; i <= rear; i++)
				{
					std::cout << this->arr[i] << " ";
				}
			}
		}

		std::cout << "  ( Size: " << _Size() << " Capacity: " << this->capacity << " Front: " << front << " Rear: " << rear << " )";
		//std::cout << std::endl;
		/*std::cout << "\tArray :";
		for (int i = 0; i < this->capacity; i++)
		{
			std::cout << (i == front ? " <<< ": "") << this->arr[i] << (i == rear ? " >>> " : "") << " ";
		}
	std::cout << std::endl;*/
			
		std::cout << std::endl;
	}
public:
	ThreadSafeQueue(int cap): ThreadSafeArray<T>(cap)
	{
		count = 0;
		rear = -1;
		front = 0;
	}

	bool Push(const T& val)
	{
		std::lock_guard<std::mutex> guard(this->m);
		if (_IsFull())
		{
			int newCapacity = std::min(this->capacity * 3 / 2 + 1, max_size);
			if (newCapacity > this->capacity)
			{
				T* newArr = new T[newCapacity];
				memset(newArr, -1, newCapacity * sizeof(T));
				
				memcpy(newArr , this->arr + front, (this->capacity - front) * sizeof(T));
				memcpy(newArr + (this->capacity - front), this->arr, (front) * sizeof(T));
				delete[] this->arr;
				this->capacity = newCapacity;
				this->arr = newArr;
				front = 0;
				rear = count - 1;
			}
			else
			{
				return false;
			}
		}
		rear = (rear + 1) % this->capacity;
		this->arr[rear] = val;
		count++;
		return true;
	}

	bool Pop(T& outVal)
	{
		std::lock_guard<std::mutex> guard(this->m);
		if (_IsEmpty())
		{
			return false;
		}
		else
		{
			outVal = this->arr[front];
			this->arr[front] = -1;
			front = (front + 1) % this->capacity;
			count--;

			if (count == 0)// Reset queue
			{
				delete[] this->arr;
				this->capacity = min_size;
				this->arr = new T[min_size];
				rear = -1;
				front = 0;
			}

			if ( count > 0 && count < (this->capacity / 3) - 1)
			{
				int newCapacity = std::max(this->capacity / 2, min_size);
				if (newCapacity < this->capacity)
				{
					T* newArr = new T[newCapacity];
					memset(newArr, -1, newCapacity * sizeof(T));

					if (rear >= front)
					{
						memcpy(newArr, this->arr + front, count * sizeof(T));
					}
					else
					{
						memcpy(newArr, this->arr + front, (this->capacity - front) * sizeof(T));
						memcpy(newArr + (this->capacity - front), this->arr, (rear+1) * sizeof(T));
					}

					delete[] this->arr;
					this->capacity = newCapacity;
					this->arr = newArr;
					front = 0;
					rear = count - 1;

				}
			}
			return true;
		}
	}

	bool IsFull()
	{
		std::lock_guard<std::mutex> guard(this->m);
		return (count == this->capacity);
	}

	bool IsEmpty()
	{
		std::lock_guard<std::mutex> guard(this->m);
		return (count == 0);
	}

	int Size()
	{
		std::lock_guard<std::mutex> guard(this->m);
		return count;
	}

	void Print()
	{
		std::lock_guard<std::mutex> guard(this->m);
		_Print();
	}

};

#endif // _ThreadSafeQueue__

