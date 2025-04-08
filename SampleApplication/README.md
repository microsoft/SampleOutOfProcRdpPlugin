This project simulates an application running in the remote machine to which the user RDPs into. 

We do the following in the project,
  1. Create a virtual channel named `SamplePluginChannel` using `WTSVirtualChannelOpenEx` API.
  2. Sends a message to the client side by writing to the virtual 
     channel using the `WTSVirtualChannelWrite` API.
  3. It also keeps checking if data is available in the virtual channel which might be sent by the
     client using the `WTSVirtualChannelRead` API.
  4. Finally it closes the virtual channel.

Note: there is a loop in point 3 above which keeps running on a separate thread. For this sample 
we never end the loop and hence, never reach point 4. But, in real world we will need to gracefully 
exit such loops.

# Links:
1. https://learn.microsoft.com/en-us/windows/win32/termserv/virtual-channel-server-application