#include "pch.h"
#include <conio.h>
#include <thread>
#include <functional> // Include this header for std::bind
#include "Utils.h"

using namespace winrt;
using namespace Windows::Foundation;

int main()
{
    std::cout << "Starting SampleApplication" << std::endl;
    int _;
    std::cout << "Initializing the COM apartment" << std::endl;
    winrt::init_apartment();
    _ = _getch();

    // DWORD sessionId = -1;
    // ProcessIdToSessionId(GetCurrentProcessId(), &sessionId);
    // std::cout << "Process id:" << GetCurrentProcessId() << ". Current RDP Session ID : " << sessionId << std::endl;

    std::cout << "Opening channel named SamplePluginChannel" << std::endl;
    std::string channelName = "SamplePluginChannel";
    // std::string channelName = "rdpdr";

    HANDLE virtualChannelHandle = WTSVirtualChannelOpenEx(WTS_CURRENT_SESSION, channelName.data(), WTS_CHANNEL_OPTION_DYNAMIC);

    _ = _getch();

    if (virtualChannelHandle == NULL) {
        std::cerr << "Failed to open virtual channel. Error: " << GetLastError() << std::endl;
        _ = _getch();
        return 1;
    }
    std::cout << "Connected to virtual channel: " << channelName << std::endl;

    // Send data to the plugin
    std::string messageToSendToPlugin;
    std::cout << "What message you want to send to the plugin?" << std::endl;
    std::cin >> messageToSendToPlugin;

    std::cout << "Initialzing Utils" << std::endl;
    auto& utils = Utils::getInstance();
    _ = _getch();

    std::cout << "Sending message to the plugin" << std::endl;
    utils.WriteToVirtualChannel(virtualChannelHandle, messageToSendToPlugin);
    _ = _getch();

    // Constantly read data from the plugin in loop in a different thread. Since, this is a new thread and uses APIs like 
    // WTSVirtualChannelRead, we should explicitly initialize the apartment.
    std::cout << "Reading data from the plugin" << std::endl;
    std::thread readerThread([&utils](HANDLE hChannel) {
        winrt::init_apartment();
        utils.ReadFromVirtualChannel(hChannel);
        }, virtualChannelHandle);
    _ = _getch();

    readerThread.join();

    std::cout << "Closing the virtual channel" << std::endl;
    WTSVirtualChannelClose(virtualChannelHandle);
    _ = _getch();

    // To hold the console window to see the output.
    std::cout << "Press any key to stop the program." << std::endl;
    _ = _getch();

    return 0;
}
