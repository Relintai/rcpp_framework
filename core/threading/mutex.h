#ifndef MUTEX_H
#define MUTEX_H

#include <mutex>

class Mutex {
public:
    void lock();
    void unlock();

    Mutex();
    virtual ~Mutex();

protected:
    std::mutex _mutex;
};


#endif