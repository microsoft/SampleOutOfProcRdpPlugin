This project simulates an RDP client. 
1. In this project we use `AppExtension` to discover the installed RDP plugins using `com.microsoft.rdp.plugin.wtsplugin`. 
   In this solution for example, it discovers the interface `IWTSPluginFactory`'s COM class implementation i.e. `CToyPlugin`.
2. We then load this class (`CToyPlugin`) using COMs `CoCreateInstance` API.
3. Finally we call the `IWTSPluginFactory::CreatePlugin` method to create the actual `IWTSPlugin`.
   The `IWTSPluginFactory::CreatePlugin` (in the `SamplePlugin` project) method has the actual code `MakeAndInitialize<CToyPluginImpl>()` code which creates the plugin.