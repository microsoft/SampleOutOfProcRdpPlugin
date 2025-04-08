# About
This is a solution that shows end to end working of a out of proc Remote Desktop Plugin that is packaged as an msix package. 
The plugin is loaded by a client application that is running on the client machine. The plugin is a COM server and the client application is a COM client. 
The plugin is discovered and loaded by the client application using the App Extension model.

## Proper Nouns
1. `SamplePluginChannel`: DVC name used by `SampleApplication` and `SamplePlugin` to communicate.
2. `com.microsoft.rdp.plugin.wtsplugin`: App Extension name that can be used by clients to load plugins.
3. `SamplePlugin`: Display name of the plugin msix.
4. `7B426A55-B731-482B-9D35-CF2A810712CB`: CLSID of the COM class that the `SamplePluginMsix` invokes.

## Solution Structure
1. `Client`: This is the client application running in client machine that loads the plugins. This is the COM client.
2. `SampleApplication`: This is the server (remote) side application for which the plugin is built.
3. `SamplePlugin`: This is the plugin that is built as an msix package. This runs in the client machine and is the COM server.
4. `SamplePluginMsix`: This is the project that packages the `SamplePlugin` as an msix package.
5. `SamplePluginProxyStub`: Since, our plugin interfaces inherit from `IUnknown`, ole automation is not possible. So, we need to create a proxy/stub dll to do custom marshalling. This project creates the proxy/stub dll.

## Building and running the solution
1. The order in which the projects should be built has been setup already. So, just build the solution and the projects should be built in the correct order.
2. Publish the SamplePlugin msix.
    * Right click on SamplePluginMsix project and click on "Publish" -> "Create App Packages"."
    * "Sideloading" -> Select a self signing certificate ("Yes, use the current certificate"). 
      More details on how to create the certificate is in `README.md` of `SamplePluginMsix`. -> x64, Release (x64) -> "Create".
3. A popup will open up which will show the location of the msix package. Install the msix package by double clicking on it.
4. Run the client executable located at "$(SolutionDir)\x64\Release\Client.exe". This should start the Client application which should load the plugins-- a terminal console should pop up.

# Disclaimer
1. This solution is only meant to be run with Release/x64 configuration.
2. This solution is not meant to be run in a production environment. This is just "a way" to write out of proc RDP plugins packaged as msix. 
   This is by no means the best way to write RDP plugins. For example, this plugin replaces the boilerplate COM APIs like AddRef, Release, QueryInterface with Windows Runtime C++ Template Library (WRL). One might do the same using WinRT, ATL, etc.
3. The solution has not been tested for security vulnerabilities.


# Maintainer Notes
- Updating SUPPORT.MD with content about this project's support experience
- Understanding the security reporting process in SECURITY.MD

## Contributing

This project welcomes contributions and suggestions.  Most contributions require you to agree to a
Contributor License Agreement (CLA) declaring that you have the right to, and actually do, grant us
the rights to use your contribution. For details, visit https://cla.opensource.microsoft.com.

When you submit a pull request, a CLA bot will automatically determine whether you need to provide
a CLA and decorate the PR appropriately (e.g., status check, comment). Simply follow the instructions
provided by the bot. You will only need to do this once across all repos using our CLA.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.

## Trademarks

This project may contain trademarks or logos for projects, products, or services. Authorized use of Microsoft 
trademarks or logos is subject to and must follow 
[Microsoft's Trademark & Brand Guidelines](https://www.microsoft.com/en-us/legal/intellectualproperty/trademarks/usage/general).
Use of Microsoft trademarks or logos in modified versions of this project must not cause confusion or imply Microsoft sponsorship.
Any use of third-party trademarks or logos are subject to those third-party's policies.
