#include "pch.h"
#include <conio.h>
#include <thread>
#include <functional> // Include this header for bind
#include "Utils.h"

using namespace winrt;
using namespace Windows::Foundation;

using namespace std;

int main()
{
    cout << "Starting SampleApplication" << endl;
    int _;
    cout << "Initializing the COM apartment" << endl;
    winrt::init_apartment();
    _ = _getch();

    // DWORD sessionId = -1;
    // ProcessIdToSessionId(GetCurrentProcessId(), &sessionId);
    // cout << "Process id:" << GetCurrentProcessId() << ". Current RDP Session ID : " << sessionId << endl;

    cout << "Opening channel named SamplePluginChannel" << endl;
    string channelName = "SamplePluginChannel";
    // string channelName = "rdpdr";

    HANDLE virtualChannelHandle = WTSVirtualChannelOpenEx(WTS_CURRENT_SESSION, channelName.data(), WTS_CHANNEL_OPTION_DYNAMIC);

    _ = _getch();

    if (virtualChannelHandle == NULL) {
        cerr << "Failed to open virtual channel. Error: " << GetLastError() << endl;
        _ = _getch();
        return 1;
    }
    cout << "Connected to virtual channel: " << channelName << endl;

    // Send data to the plugin
    string messageToSendToPlugin;
    cout << "What message you want to send to the plugin?" << endl;
    cin >> messageToSendToPlugin;

    int firstNumber = 0;
    int secondNumber = 0;
    cout << "Enter two numbers to add (separated by a space): " << endl;
    cin >> firstNumber >> secondNumber;

    string payloadToSendToPlugin = messageToSendToPlugin + " " + to_string(firstNumber) + " " + to_string(secondNumber);

    cout << "Initialzing Utils" << endl;
    auto& utils = Utils::getInstance();
    _ = _getch();

    cout << "Sending message to the plugin" << endl;
    utils.WriteToVirtualChannel(virtualChannelHandle, payloadToSendToPlugin);
    _ = _getch();

    // Constantly read data from the plugin in loop in a different thread. Since, this is a new thread and uses APIs like 
    // WTSVirtualChannelRead, we should explicitly initialize the apartment.
    cout << "Reading data from the plugin" << endl;
    thread readerThread([&utils](HANDLE hChannel) {
        winrt::init_apartment();
        utils.ReadFromVirtualChannel(hChannel);
        }, virtualChannelHandle);
    _ = _getch();

    readerThread.join();

    cout << "Closing the virtual channel" << endl;
    WTSVirtualChannelClose(virtualChannelHandle);
    _ = _getch();

    // To hold the console window to see the output.
    cout << "Press any key to stop the program." << endl;
    _ = _getch();

    return 0;
}
