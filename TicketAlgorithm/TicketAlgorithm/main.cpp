#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include <sstream>
#include "main.h"

const int n = 10;
std::atomic_int number;
std::atomic_int next;
std::atomic_int turn[n];
std::mutex Mutex;
int numThreads;
std::ostringstream data;


void func() 
{	
	
    int i = numThreads++; 

    Mutex.lock();
    std::cout << "Thread " << i << " reporting in." << std::endl;
    Mutex.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 500 + 500));

    while (true)
    {
        turn[i] = number.fetch_add(1);

        Mutex.lock();
        std::cout << "t" << i << "turn: " << turn[i] << std::endl;
        Mutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 500 + 500));

        while (turn[i] != next)
        {
            continue;
        }

        Mutex.lock();
        std::cout << "t" << i << "\t+CS" << std::endl;
        Mutex.unlock();

        data << " data" << i;

        next += 1;

        Mutex.lock();
        std::cout << data.str() << std::endl;
        std::cout << "t" << i << "next = " << next << std::endl;
        Mutex.unlock();
    }
}


int main(int argc, char* argv[])
{
    srand(time(NULL));

    data = std::ostringstream();

    numThreads = 0;
    number = 1;
    next = 1;

    for (int i = 0; i < n; i++)
    {
        turn[i] = 0;
    }

    std::thread t1 = std::thread(func);
    t1.join();
   
    return 0;
}