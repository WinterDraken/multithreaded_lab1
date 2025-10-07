#include "FairShareMutex.hpp"


//This is for writng, only write when you have 
//no readers and a writer
void FairShareMutex::lock(){
    std::unique_lock<std::mutex> ql(queue_lock);
    waiting_writers++;

    cond.wait(ql, [this](){
        return active_readers == 0 && !active_writer;
    });

    waiting_writers--;
    active_writer = true;
    ql.unlock();

    data_lock.lock();


}


//This is for unlocking the writer lock 
void FairShareMutex::unlock(){
    
    data_lock.unlock();

    {
        std::lock_guard<std::mutex> ql(queue_lock);
        active_writer = false;
        cond.notify_all();
    }

    
}

//This is for readers, share lock when you start reading
void FairShareMutex::shared_lock(){
    std::unique_lock<std::mutex> ql(queue_lock);

    cond.wait(ql, [this](){
        return !active_writer && waiting_writers == 0;
    });

    active_readers++;
    ql.unlock();
    data_lock.lock_shared();


}

//This is for unlocking readers
void FairShareMutex::shared_unlock(){
    
    data_lock.unlock_shared();
    
    {
        std::lock_guard<std::mutex> ql(queue_lock);
        active_readers--;
        if(active_readers == 0){
            cond.notify_all();
        }
    }

    

}