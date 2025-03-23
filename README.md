# Project

## Proper Nouns
1. `SamplePluginChannel`: DVC name used by `SampleApplication` and `SamplePlugin` to communicate.
2. `com.microsoft.rdp.plugin.wtsplugin`: 
3. `SamplePlugin`: Display name of the plugin msix.
4. `7B426A55-B731-482B-9D35-CF2A810712CB`: CLSID of the COM class that the `SamplePluginMsix` invokes.

## Disclaimer
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
