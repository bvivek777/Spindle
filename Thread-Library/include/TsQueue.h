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
    std::mutex lock;
public:
    /* Default Constructor */
    TsQueue();

    /* Pushes to the front of the queue
     * arg : reference of type <T>
     * return : int ( current queue size / current entered position )
     */
    int pushFront(T &front);

    /* Pushes to the back of the queue 
     * arg : reference of type <T>
     * return : int ( current queue size / current entered position )
     */
    int pushBack(T &back);

    /* Deletes from the front of the queue
     * arg : NULL (doesn't take any arguments) 
     * return : *T ( pointer to the first object )
     */
    T* popFront();

    /* Deletes from the back of the queue
     * arg : NULL (doesn't take any arguments) 
     * return : *T ( pointer to the last object )
     */
    T* popBack();

    /* Returns the total size of the queue 
     * arg : NULL (doesn't take any arguments) 
     * return : int ( size of the queue )
     */
    int size();

    bool empty();

    /* Deallocates and clears the queue */
    ~TsQueue<T>();
};
#endif