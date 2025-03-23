#pragma once
class Utils
{
public:

    static Utils& getInstance()
    {
        static Utils instance;
        return instance;
    }

    // Singleton so don't allow copy constructor or assignment operator
    Utils(const Utils&) = delete;
    Utils& operator=(const Utils&) = delete;

    void ReadFromVirtualChannel(HANDLE hChannel);
    void WriteToVirtualChannel(HANDLE hChannel, const std::string& message);

private:
    // Singleton so don't allow constructor or destructor
    Utils() = default;
    ~Utils() = default;
};

