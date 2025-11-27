#ifndef __TS_QUEUE__
#define __TS_QUEUE__

#include <deque>
#include <mutex>
#include <functional>
#include <utility>
#include <algorithm>
#include <memory>

typedef long long int ll;

struct FunctionToId
{
    void (*funcPtr)();
    ll id;
    
    FunctionToId() : funcPtr(nullptr), id(0) {}
    
    FunctionToId(void (*funcptr)(), ll id_a) : funcPtr(funcptr), id(id_a) {}
    
    // Move constructor
    FunctionToId(FunctionToId&& other) noexcept 
        : funcPtr(other.funcPtr), id(other.id) {
        other.funcPtr = nullptr;
    }
    
    // Move assignment
    FunctionToId& operator=(FunctionToId&& other) noexcept {
        if (this != &other) {
            funcPtr = other.funcPtr;
            id = other.id;
            other.funcPtr = nullptr;
        }
        return *this;
    }
    
    // Copy constructor
    FunctionToId(const FunctionToId& other) = default;
    
    // Copy assignment
    FunctionToId& operator=(const FunctionToId& other) = default;
    
    bool isValid() const {
        return funcPtr != nullptr;
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
    mutable std::mutex mutex;
public:
    /* Default Constructor */
    TsQueue(){};
   // TsQueue(int a);
    /* Pushes to the front of the queue
     * arg : reference of type <T>
     * return : int ( current queue size / current entered position )
     */
    int pushFront(const T &front){
        std::lock_guard<std::mutex> lock(mutex);
        pendingQueue.push_front(front);
        return static_cast<int>(pendingQueue.size()); 
    }
   // int pushFront(T &front, int a);
    /* Pushes to the back of the queue 
     * arg : reference of type <T>
     * return : int ( current queue size / current entered position )
     */
    int pushBack(const T &front){
        std::lock_guard<std::mutex> lock(mutex);
        pendingQueue.push_back(front);
        return static_cast<int>(pendingQueue.size()); 
    }
    
    /* Emplace to the back of the queue (more efficient for non-copyable types)
     * arg : arguments to construct T
     * return : int ( current queue size )
     */
    template<typename... Args>
    int emplaceBack(Args&&... args) {
        std::lock_guard<std::mutex> lock(mutex);
        pendingQueue.emplace_back(std::forward<Args>(args)...);
        return static_cast<int>(pendingQueue.size());
    }
   // int pushBack(T &front, int a);
    /* Deletes from the front of the queue
     * arg : NULL (doesn't take any arguments) 
     * return : T ( copy of the first object, default-constructed if empty )
     */
    T popFront() {
        std::lock_guard<std::mutex> lock(mutex);
        if (pendingQueue.empty()) {
            return T{};
        }
        T t = std::move(pendingQueue.front());
        pendingQueue.pop_front();
        return t;
    }
    
    /* Try to pop from front, returns false if empty
     * arg : reference to T to store result
     * return : bool ( true if successful, false if empty )
     */
    bool tryPopFront(T& result) {
        std::lock_guard<std::mutex> lock(mutex);
        if (pendingQueue.empty()) {
            return false;
        }
        result = std::move(pendingQueue.front());
        pendingQueue.pop_front();
        return true;
    }
   // T popFront(int a); 
    /* Deletes from the back of the queue
     * arg : NULL (doesn't take any arguments) 
     * return : T ( copy of the last object, default-constructed if empty )
     */
    T popBack(){
        std::lock_guard<std::mutex> lock(mutex);
        if (pendingQueue.empty()) {
            return T{};
        }
        T t = std::move(pendingQueue.back());
        pendingQueue.pop_back();
        return t;
    }
   // T popBack(int a);
    /* Returns the total size of the queue 
     * arg : NULL (doesn't take any arguments) 
     * return : size_t ( size of the queue )
     */
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex);
        return pendingQueue.size();
    }
   // int size(int a);
     bool empty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return pendingQueue.empty();
    }
    
    /* Clear all elements from the queue */
    void clear() {
        std::lock_guard<std::mutex> lock(mutex);
        pendingQueue.clear();
    }
   // bool empty(int a);
    /* Deallocates and clears the queue */
    ~TsQueue(){};

};
#endif