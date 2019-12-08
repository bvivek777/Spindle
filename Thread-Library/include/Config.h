class Config {
private:
    static int instanceCount;
public:    
    Config(/* args */);
    ~Config();
};

int Config::instanceCount = 0;