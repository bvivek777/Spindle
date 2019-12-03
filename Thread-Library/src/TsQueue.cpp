#include "TsQueue.h"

template<typename T>
TsQueue<T>::TsQueue(/* args */) {
}

template<typename T>
TsQueue<T>::~TsQueue() {
}

template<typename T>
int TsQueue<T>::pushFront(T &front){
    auto lock = std::unique_lock<std::mutex>(this.mutex);
    pendingQueue.push_back(front);
    return pendingQueue.size(); 
}

template<typename T>
int TsQueue<T>::pushBack(T &front){
    auto lock = std::unique_lock<std::mutex>(this.mutex);
    pendingQueue.push_back(front);
    return pendingQueue.size(); 
}

template<typename T>
T* TsQueue<T>::popBack(){
    auto lock = std::unique_lock<std::mutex>(this.mutex);
    T *t = NULL;
    if ( !pendingQueue.size() )
        return t;
    t = pendingQueue.back();
    pendingQueue.pop_back();
    return T;
}

template<typename T>
T* TsQueue<T>::popFront() {
    auto lock = std::unique_lock<std::mutex>(this.lock);
    T *t = NULL;
    if ( !pendingQueue.size() )
        return t;
    t = pendingQueue.front();
    pendingQueue.pop_front();
    return t;
}

template<typename T>
int TsQueue<T>::size(){
    auto lock = std::unique_lock<std::mutex>(this.lock);
    return pendingQueue.size();
}
