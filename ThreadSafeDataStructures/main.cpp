#include <iostream>
#include <cstdlib> 
#include <ctime>
#include <thread>
#include <chrono>
#include "ThreadSafeArray.h"
#include "ThreadSafeStack.h"
#include "ThreadSafeQueue.h"

#define WAIT_DURATION 1 // Wait for next process at own thread;
#define WAIT_OTHER_THREAD_DURATION 100 // Thread waits for other threat.
#define LOG_DURATION 1000 // Log time interval
#define WAIT_LONG 2000 // Thread waits like it has stopped
#define RUN_TIME_INTERVAL 15 // Thread waits like it has stopped


void Supplier(ThreadSafeArray<int>& tss)
{
	std::clock_t begin, end, restart;
	double elapsed_secs, elapsed_since_start;
	begin = clock();
	restart = begin;
	int loggedBefore = false;
	while(true)
	{
		end = clock();

		elapsed_secs = double(end - restart) / CLOCKS_PER_SEC;
		if (elapsed_secs > 5.0f)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_LONG));
			restart = clock();
		}

		elapsed_since_start = double(end - begin) / CLOCKS_PER_SEC;
		if (elapsed_since_start >= RUN_TIME_INTERVAL)
		{
			std::cout << "Exiting supplier thread" << std::endl;
			break;
		}

		int val = ( rand() % 100) + 1;
		if (!tss.Push(val))
		{
			if (!loggedBefore)
			{
				loggedBefore = true;
				//std::cout << "Stack is full, waiting for consumer thread" << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_OTHER_THREAD_DURATION));
		}
		else
		{
			loggedBefore = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds( ( ( WAIT_DURATION ) / 2 ))); // make supplier a bit faster ( wait less )
	}
}

void Consumer(ThreadSafeArray<int>& tss)
{
	std::clock_t begin, end;
	double elapsed_secs;
	begin = clock();

	int loggedBefore = false;
	while (true)
	{
		end = clock();
		elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		if (elapsed_secs >= RUN_TIME_INTERVAL)
		{
			std::cout << "Exiting consumer thread" << std::endl;
			break;
		}

		int val;
		if (!tss.Pop(val))
		{
			if (!loggedBefore)
			{
				loggedBefore = true;
				//std::cout << "Stack is empty, waiting for supplier thread" << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_OTHER_THREAD_DURATION));
		}
		else
		{
			loggedBefore = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds( (WAIT_DURATION ) ) );
	}
}

void Tracker(ThreadSafeArray<int>& tss)
{
	std::clock_t begin, end;
	double elapsed_secs;
	begin = clock();
	while (true)
	{
		tss.Print();
		std::this_thread::sleep_for(std::chrono::milliseconds(LOG_DURATION));
		end = clock();
		elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		if (elapsed_secs >= RUN_TIME_INTERVAL)
		{
			std::cout << "Exiting tracker thread" << std::endl;
			break; 
		}
	}
}

int main()
{
	srand(time(NULL));
	ThreadSafeQueue<int> s(10);
	std::thread t1(Supplier, std::ref(s));// std::thread can not understand if 's' is a reference or value so we implictly show it is a ref. 
	std::thread t2(Supplier, std::ref(s));
	std::thread t3(Supplier, std::ref(s));
	std::thread t4(Consumer, std::ref(s));
	std::thread t5(Consumer, std::ref(s));
	std::thread t6(Tracker, std::ref(s)); 
	
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	s.Print();
	/*
	int val;
	ThreadSafeQueue<int> tsq(4);
	for (int i = 0; i < 6; i++)
	{
		tsq.Push((rand() % 100) + 1);
		tsq.Print();
	}
	for (int i = 0; i < 7; i++)
	{
		tsq.Pop(val);
		tsq.Print();
	}
	for (int i = 0; i < 18; i++)
	{
		tsq.Push((rand() % 100) + 1);
		tsq.Print();
	}
	tsq.Print();*/
	system("pause");
	return 0;
}