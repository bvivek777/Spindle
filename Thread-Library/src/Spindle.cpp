#include "Spindle.h"

template <typename T>
Spindle<T>::Spindle(T* funcPtr, int n)
            : funcPtr(funcPtr)
            , num_Threads(n)
{
    for(int i = 0; i< num_Threads; i++)
        workerThreads.push_back(std::thread(&funcPtr));
}

template <typename T>
Spindle<T>::~Spindle()
{
    for(auto thread : workerThreads)
    {
        if(thread.joinable())
            thread.join();
    }
}

template <typename T>
bool Spindle<T>::Add(T* funcPtr)
{
    try
    {
        workerThreads.push_back(std::thread(&funcPtr));
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
}

template <typename T>
bool Spindle<T>::Add_n_Threads(T* funcPtr, int n)
{
    try
    {
        for(int i = 0; i < n; i++)
        {
            workerThreads.push_back(std::thread(&funcPtr));
        }
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}