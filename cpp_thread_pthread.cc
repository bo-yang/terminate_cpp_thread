#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <cstring>
#include <pthread.h>
 
std::mutex iomutex;
void f(int num)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::lock_guard<std::mutex> lk(iomutex);
    std::cout << "Thread " << num << " pthread_t " << pthread_self() << std::endl;
}
 
int main()
{
    std::thread t1(f, 1), t2(f, 2);
    
    //t1.join(); t2.join();
    //t1.detach(); t2.detach();
    
    std::cout << "Thread 1 thread id " << t1.get_id() << std::endl;
    std::cout << "Thread 2 thread id " << t2.get_id() << std::endl;
    
    std::cout << "Thread 1 native handle " << t1.native_handle() << std::endl;
    std::cout << "Thread 2 native handle " << t2.native_handle() << std::endl;
    
    t1.join(); t2.join();
    //t1.detach(); t2.detach();
}

/* Before thread join/detach:
Thread 1 thread id 140280111331072
Thread 2 thread id 140280102938368
Thread 1 native handle 140280111331072
Thread 2 native handle 140280102938368
Thread 1 pthread_t 140280111331072
Thread 2 pthread_t 140280102938368
 */

/* After thread join:
Thread 1 pthread_t 139811504355072
Thread 2 pthread_t 139811495962368
Thread 1 thread id thread::id of a non-executing thread
Thread 2 thread id thread::id of a non-executing thread
Thread 1 native handle 0
Thread 2 native handle 0
 */

/* Before thread detach:
Thread 1 thread id 140387472209664
Thread 2 thread id 140387463816960
Thread 1 native handle 140387472209664
Thread 2 native handle 140387463816960
*/

/* After thread detach:
Thread 1 thread id thread::id of a non-executing thread
Thread 2 thread id thread::id of a non-executing thread
Thread 1 native handle 0
Thread 2 native handle 0
*/
