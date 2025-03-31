#include "pch.h"
#include "Utils.h"
#include <conio.h>

int main()
{
    std::cout << "ClientApp started." << std::endl;
    char _;
    _ = _getch();

    auto &utils = Utils::getInstance();

    // Initialize the plugins.
    std::vector<CLSID> classIds  = utils.GetRdpPluginClassIds();
    std::cout << "RDP Plugin class ids fetched." << std::endl;
    utils.PrintPluginComClsIds(classIds);

    std::cout << "Press any key to load all plugins." << std::endl;
    _ = _getch();
    utils.LoadAllSamplePlugins(classIds);
    std::cout << "All plugins successfully loaded." << std::endl;

    // Wait for user input before closing the program.
    std::cout << "Closing the app now on pressing of any key." << std::endl;
    _ = _getch();

    // Close the plugins.
    utils.Terminate();

    return 0;
}
