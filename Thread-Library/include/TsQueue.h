#ifndef __TS_QUEUE__
#define __TS_QUEUE__

#include <deque>
#include <mutex>
#include <functional>

typedef long long int ll;

struct FunctionToId
{
    void (*funcPtr)();
    ll id;
    FunctionToId(){}
    FunctionToId(void (*funcptr)(), ll id_a){
        funcPtr = funcptr;
        id = id_a;
    }
};

/*
 * A thread safe Double ended queue, which will be used to store
 * the void pointers from the functions which have to be parallelized
 */
template<typename T>
class TsQueue {
private:
    std::deque<T> pendingQueue;
    std::mutex mutex;
public:
    /* Default Constructor */
    TsQueue(){};
   // TsQueue(int a);
    /* Pushes to the front of the queue
     * arg : reference of type <T>
     * return : int ( current queue size / current entered position )
     */
    int pushFront(T &front){
        auto lock = std::unique_lock<std::mutex>(mutex);
        pendingQueue.push_back(front);
        return pendingQueue.size(); 
    }
   // int pushFront(T &front, int a);
    /* Pushes to the back of the queue 
     * arg : reference of type <T>
     * return : int ( current queue size / current entered position )
     */
    int pushBack(T &front){
        auto lock = std::unique_lock<std::mutex>(mutex);
        pendingQueue.push_back(front);
        return pendingQueue.size(); 
    }
   // int pushBack(T &front, int a);
    /* Deletes from the front of the queue
     * arg : NULL (doesn't take any arguments) 
     * return : *T ( pointer to the first object )
     */
    T popFront() {
        auto lock = std::unique_lock<std::mutex>(mutex);
        T t;
        if ( !pendingQueue.size() )
            return t;
        t = pendingQueue.front();
        pendingQueue.pop_front();
        return t;
    }
   // T popFront(int a); 
    /* Deletes from the back of the queue
     * arg : NULL (doesn't take any arguments) 
     * return : *T ( pointer to the last object )
     */
    T popBack(){
        auto lock = std::unique_lock<std::mutex>(mutex);
        T t;
        if ( !pendingQueue.size() )
            return t;
        t = pendingQueue.back();
        pendingQueue.pop_back();
        return t;
    }
   // T popBack(int a);
    /* Returns the total size of the queue 
     * arg : NULL (doesn't take any arguments) 
     * return : int ( size of the queue )
     */
    int size(){
        auto lock = std::unique_lock<std::mutex>(mutex);
        return pendingQueue.size();
    }
   // int size(int a);
    bool empty(){
        auto lock = std::unique_lock<std::mutex>(mutex);
        return pendingQueue.empty();
    }
   // bool empty(int a);
    /* Deallocates and clears the queue */
    ~TsQueue(){};

};
#endif