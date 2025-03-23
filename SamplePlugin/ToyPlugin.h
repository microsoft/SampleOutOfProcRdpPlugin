#pragma once

#include "pch.h"
#include "../MidlGeneratedFiles/IWTSPlugin_h.h"
#include <wrl/module.h>
#include <wrl/implements.h>

using namespace Microsoft::WRL;

class __declspec(uuid("7B426A55-B731-482B-9D35-CF2A810712CB")) CToyPlugin
	: public RuntimeClass<RuntimeClassFlags<ClassicCom>, IWTSPluginFactory>
{
public:
	IFACEMETHODIMP CreatePlugin(long index, IWTSPlugin** plugin) override;
	IFACEMETHODIMP GetPluginCount(long* count) override;
};