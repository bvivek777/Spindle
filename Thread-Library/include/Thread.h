#include <thread>
#include <functional>
#include "TsQueue.h"

class Thread
{
    private:
        std::thread::id tid;
        std::thread thread;
        TsQueue<std::function<void(int function)>*> processPool;
    public:
        Thread();
        template<typename T>
        bool AddToQueue(T* funcPtr);
};