#pragma once
#include <wrl.h>
#include "../MidlGeneratedFiles/IWTSPlugin_h.h"

using namespace Microsoft::WRL;

class CToyPluginWindowChangedCallbackImpl
    : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IWTSWindowChangedCallback>
{
public:
    CToyPluginWindowChangedCallbackImpl() = default;

    IFACEMETHODIMP WindowChanged(const WTSWindowInfo* windowInfo) override;
};
