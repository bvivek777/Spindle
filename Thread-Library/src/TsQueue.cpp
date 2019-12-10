/* #include "../include/TsQueue.h"

template<typename T>
TsQueue<T>::TsQueue(int a){

}

template<typename T>
int TsQueue<T>::pushFront(T &front, int a){
    auto lock = std::unique_lock<std::mutex>(this.mutex);
    pendingQueue.push_back(front);
    return pendingQueue.size(); 
}

template<typename T>
int TsQueue<T>::pushBack(T &front, int a){
    auto lock = std::unique_lock<std::mutex>(this.mutex);
    pendingQueue.push_back(front);
    return pendingQueue.size(); 
}

template<typename T>
T TsQueue<T>::popBack(int a){
    auto lock = std::unique_lock<std::mutex>(this.mutex);
    T *t = NULL;
    if ( !pendingQueue.size() )
        return t;
    t = pendingQueue.back();
    pendingQueue.pop_back();
    return t;
}

template<typename T>
T TsQueue<T>::popFront(int a) {
    auto lock = std::unique_lock<std::mutex>(this.lock);
    T *t = NULL;
    if ( !pendingQueue.size() )
        return t;
    t = pendingQueue.front();
    pendingQueue.pop_front();
    return t;
}

template<typename T>
int TsQueue<T>::size(int a){
    auto lock = std::unique_lock<std::mutex>(this.lock);
    return pendingQueue.size();
}

template<typename T>
bool TsQueue<T>::empty(int a)
{
    auto lock = std::unique_lock<std::mutex>(this.lock);
    return pendingQueue.empty();
} */