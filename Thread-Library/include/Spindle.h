#include <vector>
#include <thread>

template<typename T>
class Spindle
{
    public:
        Spindle() = default;
        // @TODO add support for arguments
        // n - number of threads to create
        Spindle(T* funcPtr, int n = 1);

        // Add another thread to the thread queue
        bool Add(T* funcPtr);

        // Add n number of threads to the thread queue
        bool Add_n_Threads(T* funcPtr, int n);

        // Destructor
        ~Spindle();
    private:
        std::vector<std::thread> workerThreads;
        int num_Threads;
        T* funcPtr;
};
