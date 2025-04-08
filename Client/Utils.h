#pragma once
#include "pch.h"
#include "../MidlGeneratedFiles/IWTSPlugin_h.h"

// A singleton class with utils funtions.
class Utils
{
public:
    static Utils& getInstance()
    {
        static Utils instance;
        return instance;
    }

    // Singleton so don't allow copy constructor or assignment operator
    Utils(const Utils&) = delete;
    Utils& operator=(const Utils&) = delete;

    std::vector<CLSID> GetRdpPluginClassIds();
    void LoadAllSamplePlugins(std::vector<CLSID>& classIds);
    HRESULT LoadOneSamplePlugin(__in CLSID pluginClassId);
    HRESULT AddPluginWithFactory(__in IWTSPlugin* pPlugin, __in IWTSPluginFactory* pFactory);
    HRESULT Terminate();
    void PrintPluginComClsIds(const std::vector<CLSID> &classIds);

private:
    // Singleton so don't allow constructor or destructor
    Utils() = default;
    ~Utils() = default;

    std::vector<IWTSPluginFactory*> ptrUserPluginsFactories;
    std::vector<IWTSPlugin*> ptrUserPlugins;
};

