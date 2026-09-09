#include "pch.h"
#include "Utils.h"

using namespace std;

void Utils::ReadFromVirtualChannel(HANDLE hChannel)
{
    char buffer[256] = { 0 };
    DWORD bytesRead = 0;

    while (true) {
        if (WTSVirtualChannelRead(hChannel, 0, buffer, sizeof(buffer) - 1, &bytesRead)) {
            buffer[bytesRead] = '\0';

            struct ChannelPduHeader { unsigned long length; unsigned long flags; };
            const DWORD headerSize = sizeof(ChannelPduHeader); // 8 bytes

            const char* payload = buffer;
            DWORD payloadBytes = bytesRead;

            if (bytesRead >= headerSize) {
                auto* header = reinterpret_cast<const ChannelPduHeader*>(buffer);
                // Single-chunk message: header.length equals the payload size (bytesRead - header).
                if (header->length == bytesRead - headerSize) {
                    payload += headerSize;
                    payloadBytes -= headerSize;
                }
            }

            string result(payload, payloadBytes);
            cout << "Message '" << result << "' recieved from the plugin. "
                      << payloadBytes << " bytes of payload (" << bytesRead
                      << " bytes on the wire including the 8-byte channel header)." << endl;
            break;
        }
        else {
            DWORD error = GetLastError();
            cerr << "Failed to read data. Error: " << error << endl;
            if (error == ERROR_NO_DATA) {
                continue;
            }
            cerr << "Failed to read from channel. Error: " << error << endl;
            break;
        }

    }
}

void Utils::WriteToVirtualChannel(HANDLE hChannel, const string& message)
{
    DWORD bytesWritten = 0;
    PCHAR pMessage = const_cast<PCHAR>(message.data());

    if (!WTSVirtualChannelWrite(hChannel, pMessage, message.size(), &bytesWritten)) {
        cerr << "Failed to write data. Error: " << GetLastError() << endl;
    }
    else {
        cout << "Message '" << message << "' sent to plugin. " << bytesWritten << " bytes of data written." << endl;
    }
}
