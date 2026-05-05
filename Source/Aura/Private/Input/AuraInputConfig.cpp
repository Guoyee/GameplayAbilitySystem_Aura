// Copyright Yerik Guo


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionByTag(const FGameplayTag& InputTag, bool EnableLog) const
{
    for (FAuraInputAction AuraIA: InputActions)
    {
        if (AuraIA.InputAction && AuraIA.InputTag == InputTag)
        {
            return AuraIA.InputAction;
        }
    }
    
    if (EnableLog)
    {
        UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
    }
    return nullptr;
}
