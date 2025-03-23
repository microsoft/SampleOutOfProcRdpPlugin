# Installing the plugin as an MSIX package

To be able to generate and install the msix, do the following.
## Create a self signing certificate
1. $FriendlyName = "SamplePluginMsixCert"
2. $MyStore = "Cert:\CurrentUser\My"
3. $SelfSignCertPassword = "<YourPassword>"
4. $SampleStringPassword = (ConvertTo-SecureString $SelfSignCertPassword  -Force -AsPlainText)
5. $CertStoreLocation = "Cert:\LocalMachine\Root"
6. $CertFilePath = "C:\Users\<UserName>\Documents\$FriendlyName.pfx"
7. $Subject = 'CN=Microsoft Corporation, O=Microsoft Corporation, L=Redmond, S=Washington, C=US'
8. $MyCert = New-SelfSignedCertificate -FriendlyName $FriendlyName -CertStoreLocation $MyStore -Type CodeSigningCert -Subject $Subject
9. $RootStore = 'Cert:\LocalMachine\Root'
10. $RootCert = Get-Item -LiteralPath (Join-Path $RootStore $MyCert.Thumbprint) -ErrorAction SilentlyContinue
11. Export-PfxCertificate -Cert $MyCert -Password $SecureStringPassword -FilePath $CertFilePath -Force
12. Import-PfxCertificate -FilePath $CertFilePath -Password $SecureStringPassword -CertStoreLocation $CertStoreLocation
13. Test-Path -Path $CertFilePath

## Sign the msix using the above certificate
Right click on `SamplePluginMsix` project -> Publish -> Create App Packages... In the prompt select the `.pfx` that was created above. This generates the `msix`.

## Install the msix now.

# What makes this msix COM capable?
In the manifest we have
```
<com:Extension Category="windows.comServer">
    <com:ComServer>
        <com:ExeServer Executable="SamplePlugin\SamplePlugin.exe">
            <com:Class Id="7B426A55-B731-482B-9D35-CF2A810712CB"></com:Class>
        </com:ExeServer>
    </com:ComServer>
</com:Extension>
```
1. `<com:Extension Category="windows.comServer">`:	Declares a COM Server in the MSIX package.
2. `<com:ExeServer Executable="SamplePlugin\SamplePlugin.exe">`: Defines the COM server binary (must be inside the package). `SamplePlugin.exe` must implement COM properly. It should register the CLSID internally via DllRegisterServer or CoRegisterClassObject.
3. `<com:Class Id="7B426A55-B731-482B-9D35-CF2A810712CB">`: Registers a specific COM class (CLSID) for this server.

Following happens because of this,
1. When the MSIX package is installed, Windows registers the COM server.
2. If an external app requests the COM class (CLSID: 7B426A55-B731-482B-9D35-CF2A810712CB), Windows launches `SamplePlugin.exe` as the COM server.


# How to extend the capabilities of our plugin by integrating a custom plugin as an App Extension which is COM capable?
```
<uap3:Extension Category="windows.appExtension">
    <uap3:AppExtension 
        Name="com.microsoft.rdp.plugin.wtsplugin"
        Id="ToyPlugin"
        DisplayName="Toy Plugin"
        Description="Toy plugin for the UWP app."
        PublicFolder="Public">
        <uap3:Properties>
            <ComServer>7B426A55-B731-482B-9D35-CF2A810712CB</ComServer>
        </uap3:Properties>
    </uap3:AppExtension>
</uap3:Extension>
```
1. `<uap3:Extension>`: Extends the functionality of an app by defining custom extensions that integrate with the Windows ecosystem. 
2. `windows.appExtension`: Category allows apps to expose plugins or extensibility points that other apps or the system can use.
3. `com.microsoft.rdp.plugin.wtsplugin`: The unique identifier for the extension.
4. `Id="ToyPlugin"`: A unique ID for this extension within the app package.
5. `PublicFolder="Public"`: Specifies a folder inside the MSIX package that contains public resources (like configs, binaries, or assets).
6. `<ComServer>7B426A55-B731-482B-9D35-CF2A810712CB</ComServer>`: This links the extension to a COM Server, which handles the plugin’s execution.

## How is the plugin is discovered and used?
1. Windows installs the MSIX package.
2️. The extension gets registered in the system.
3️. Other applications or system components query for available extensions using the AppExtension API.
4️. If a match is found (e.g., an RDP client looks for com.microsoft.rdp.plugin.wtsplugin), Windows loads the extension dynamically.

