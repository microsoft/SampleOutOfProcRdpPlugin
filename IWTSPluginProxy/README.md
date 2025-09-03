`IWTSPluginProxy` is needed for marshalling and unmarshalling the data, when it is transferred between the COM server (SamplePlugin project) and the COM client (Client project).

# How does this work?
When the MIDL compiler runs (it runs automatically when you build `IWTSPluginProxy`) while building the `IWTSPluginProxy` project, it generates the `Interfaces_p.c` file.
See https://learn.microsoft.com/en-us/windows/win32/midl/the-interface-proxy-file. This file contains routines that facilitate this marshalling and unmarshalling.

# Note
1. Since, the `IWTSPluginProxy.dll` is generated using MIDL, we don't need to explicitly register the DLL using the standard `regsvr32 IWTSPluginProxy.dll` command.
   Instead the registration of the IWTSPluginProxy.dll is done in the `SamplePluginMsix\Package.appxmanifest` file using `com:ProxyStub`. This happens automatically when the msix is installed.
2. Look at the `PreprocessorDefinitions` inside `IWTSPluginProxy.vcxproj` which tells the CLSID of the class which does the marshalling and unmarshalling.