This project is needed for marshalling and unmarshalling the data between the COM server (SamplePlugin project) and the COM client (Client project).
The plugin and the client application are running in different processes, so the data needs to be marshalled and unmarshalled between 
the two processes. The proxy stub is responsible for marshalling and unmarshalling the data between the plugin and the host application.

# How does this work?
TODO: Should this go to the MS documentation?

# Note
1. Since, the SamplePluginProxyStub.dll is generated using MIDL, we can't use the standard `regsvr32 /i SamplePluginProxyStub.dll` command because we don't have the `DllRegisterServer` entry point.
2. Instead the registration of the SamplePluginProxyStub.dll is done in the `SamplePluginMsix\Package.appxmanifest` file using `com:ProxyStub`. This happens automatically when the msix is installed.