#ifndef __HELPER__
#define __HELPER__

#include <string>
#include <fstream>

class Helper {
private:
    
public:
    Helper(/* args */);
    ~Helper();
    std::string httpCall();
    bool consoleLog();
    template<typename T>
    std::string getFromConsole();   

    std::string readFromFile(std::string path);
    bool writeToFile(std::string writeString);
    void handleException();
};

#endif
