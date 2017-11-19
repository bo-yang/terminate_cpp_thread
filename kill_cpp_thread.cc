#include <thread>
#include <string>
#include <iostream>
#include <unordered_map>
#include <unistd.h>
#include <cstdlib>
#include <pthread.h>
#include <sys/prctl.h>

// 
// A demo of killing C++ thread.
//
// Compile command:
//  g++ -Wall -std=c++11 kill_cpp_thread.cc -o kill_cpp_thread -pthread -lpthread
//  ./kill_cpp_thread
//

/* Using thread destructor
Thread test_thread1 created:
22520 22521 pts/5    00:00:00 test_thread1
Thread test_thread1 killed:
22520 22521 pts/5    00:00:00 test_thread1
Thread test_thread2 created:
22520 22521 pts/5    00:00:00 test_thread1
22520 22528 pts/5    00:00:00 test_thread2
Thread test_thread2 killed:
22520 22521 pts/5    00:00:00 test_thread1
22520 22528 pts/5    00:00:00 test_thread2
 */

/* Using pthread_cancel
Thread test_thread1 created:
22729 22730 pts/5    00:00:00 test_thread1
Thread test_thread1 killed:
Thread test_thread2 created:
22729 22737 pts/5    00:00:00 test_thread2
Thread test_thread2 killed:
 */

class Foo {
public:
    void sleep_for(const std::string &tname, int num)
    {
        prctl(PR_SET_NAME,tname.c_str(),0,0,0);        
        sleep(num);
    }

    void start_thread(const std::string &tname)
    {
        std::thread * thrd = new std::thread(&Foo::sleep_for, this, tname, 3600);
        tm_[tname] = thrd->native_handle();
        thrd->detach();
        //tm_[tname] = thrd;
        std::cout << "Thread " << tname << " created:" << std::endl;
    }

    void stop_thread(const std::string &tname)
    {
        ThreadMap::const_iterator it = tm_.find(tname);
        if (it != tm_.end()) {
            //delete it->second; // thread not killed
            //it->second->std::thread::~thread(); // thread not killed
            pthread_cancel(it->second);
            tm_.erase(tname);
            std::cout << "Thread " << tname << " killed:" << std::endl;
        }
    }

private:
    //typedef std::unordered_map<std::string, std::thread*> ThreadMap;
    typedef std::unordered_map<std::string, pthread_t> ThreadMap;
    ThreadMap tm_;
};

void show_thread(const std::string &keyword)
{
    std::string cmd("ps -T | grep ");
    cmd += keyword;
    system(cmd.c_str());
}
 
int main()
{
    Foo foo;
    std::string keyword("test_thread");
    std::string tname1 = keyword + "1";
    std::string tname2 = keyword + "2";

    // create and kill thread 1
    foo.start_thread(tname1);
    show_thread(keyword);
    foo.stop_thread(tname1);
    show_thread(keyword);

    // create and kill thread 2
    foo.start_thread(tname2);
    show_thread(keyword);
    foo.stop_thread(tname2);
    show_thread(keyword);

    return 0;
}

