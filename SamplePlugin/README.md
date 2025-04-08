This is the main RDP plugin class which implements the various IWTSPlugin interfaces like `IWTSListenerCallback`, `IWTSVirtualChannelCallback`, etc. 

Following are the details of what ahppens in the project.

1. We start by doing the standard COM setup.
 a. `CoCreatableClass(CToyPlugin)` is a macro that indicates thatCToyPlugin is a COM function and all the COM rules like reference counting need to be done for CToyPlugin
 b. CoInitializeEx: Initialize the COM library
 c. Set some properties like `FastRundown` (see Pre-requisites / Technologies to know). So set that using, `CoCreateInstance(CLSID_GlobalOptions, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&globalOptions))`
 d. Set some security policies on the out of proc SamplePlugin server using `CoInitializeSecurity`.
 e. `Module<OutOfProc>::Create(Shutdown)` creates a singleton instance of the module names OutOfProc.
 f. `module.RegisterObjects()` tells COM runtime engine to register all classes marked as `CoCreatableClassas` COM classes. In our case `CToyPlugin`.
  
2. The plugin then just keeps running until it shuts down via COM Fast Rundown i.e. when all RDP clients are done using it by releasing the references.

3. The `CToyPlugin` class implements the `IWTSPluginFactory` interface's two functions `CreatePlugin` and `GetPluginCount`. 
   RDP clients use these functions to create the plugins. As a plugin author you can assume that the RDP clients will call the `CreatePlugin` and the `GetPluginCount` functions. It is the plugin author's responsibility to write the class which implements the plugin 
   and set it to the `IWTSPlugin**` plugin variable passed into the `CreatePlugin` function by the RDP client. 
   In our example plugin, we create the plugin by creating an object of class `CToyPluginImpl` which implements `IWTSPlugin`. Similarly, the RDP client will call the `GetPluginCount` 
   function whenever it wants to see how many plugins are present in the plugin COM server process. 
   For our toy example we are always setting it to 1 (which should be the case most of the times).

4. Our plugin class `CToyPluginImpl` implements the functions of interface `IWTSPlugin` and `IWTSListenerCallback`. 
   Of these `IWTSPlugin::Initialize` and `IWTSListenerCallback::OnNewChannelConnection` are are important and will be elaborated here.
 a. `IWTSPlugin::Initialize` is called by the Remote Desktop Services subsystem and passes in the `IWTSVirtualChannelManager`. 
    The plugin author or RDP client need not create the `IWTSVirtualChannelManager` object. Here in the example plugin 
    inside the `Initialize` function, we use the COM's `QueryInterface` method to get a pointer to the new COM interface 
    `IWTSPluginServiceProvider` whose `GetService` function can be used to query various Remote Desktop Client 
    services (we have an example of this explained later in this document). We then use the `CreateListener` method 
    implemented by the Rremote Desktop Services Virtual Channel Manager which is part of Windows Remote Desktop Services API 
    to create a listener named `SamplePluginChannel`. Thus, as a plugin author you don't have to implement the 
    `CreateListener` method; remote Desktop Connection client (MSTSC.exe) does that for you.
    Note: A listener is like a port which the RDP client and the plugin use to communicate.
 b. `IWTSListenerCallback::OnNewChannelConnection` is called when a new channel connection is established 
    (not every time data is exchanged between RDP client and RDP server). 
    In this function, for the toy example, we accept the connection by setting `pbAccept`. 
    We then create an object of class `IWTSVirtualChannelCallback` which will be used when bidirectional communication 
    happens and is explained next. 

5. `CToyPluginVirtualChannelCallbackImpl::OnDataReceived` is a callback which is called whenever the RDP server sends 
   some message to the RDP plugin. 
   The data sent by the RDP server to the RDP plugin comes in `pBuffervariable`. In this sample plugin, we convert the 
   byte data to proper format and print it. We then pass a data back to the RDP server using `_pChannel->Write`. 
   This completes the bi-directional communication between the RDP plugin and the RDP server. 
   Finally, we also call the `IWTSWindowInfoService`'s `SubscribeWindowChanged` function implemented by the RDP client 
   (via COM interop) and pass along a pointer to `CToyPluginWindowChangedCallbackImpl`. 
   On the RDP client side then, we have access to the `WindowChanged` method (implemented by the RDP plugin).
   Basically, the RDP plugin uses COM's interop to call a function impelmented by the RDP client. The RDP client then uses 
   the COM's interop to call a function in the RDP plugin.
   Note: We are passing a nullptr here since, this is just a sample. 
   In real world scenario, you should pass a valid HWND of the window which we want to track.

Note: 
1. Many of the interface definitions are just basic toy implementations. 
   In the real world those implementations will be more complex. 
   These are the minimal set of definitions that every plugin will have. 
   As more functionalities are added to the plugin more interfaces need to be created and 
   defined based on use case. 
   For example, we don't define the `IWTSBitmapRenderServiceinterface` in this example.
2. In our example, we define functions inside the following interfaces,
   `IWTSPluginFactory`, `IWTSPlugin`, `IWTSListenerCallback`, `IWTSVirtualChannelCallback`, `IWTSWindowInfoService`