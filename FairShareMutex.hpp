#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <iostream>

class FairShareMutex{
    private:
        std::shared_mutex data_lock;
        std::mutex queue_lock;
        std::condition_variable cond;

        bool active_writer = false;
        int active_readers = 0;
        int waiting_writers = 0;

    public:
        void lock();
        void unlock();
        void shared_lock();
        void shared_unlock();

};