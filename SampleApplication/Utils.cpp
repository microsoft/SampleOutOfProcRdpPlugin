#include "pch.h"
#include "Utils.h"

void Utils::ReadFromVirtualChannel(HANDLE hChannel)
{
    char buffer[256] = { 0 };
    DWORD bytesRead = 0;

    while (true) {
        if (WTSVirtualChannelRead(hChannel, 0, buffer, sizeof(buffer) - 1, &bytesRead)) {
            buffer[bytesRead] = '\0';
            std::cout << "Message '" << buffer << "' recieved from the plugin. " << bytesRead << " bytes of data recieved." << std::endl;
            break;
        }
        else {
            DWORD error = GetLastError();
            std::cerr << "Failed to read data. Error: " << error << std::endl;
            if (error == ERROR_NO_DATA) {
                continue;
            }
            std::cerr << "Failed to read from channel. Error: " << error << std::endl;
            break;
        }

    }
}

void Utils::WriteToVirtualChannel(HANDLE hChannel, const std::string& message)
{
    DWORD bytesWritten = 0;
    PCHAR pMessage = const_cast<PCHAR>(message.data());

    if (!WTSVirtualChannelWrite(hChannel, pMessage, message.size(), &bytesWritten)) {
        std::cerr << "Failed to write data. Error: " << GetLastError() << std::endl;
    }
    else {
        std::cout << "Message '" << message << "' sent to plugin. " << bytesWritten << " bytes of data written." << std::endl;
    }
}
