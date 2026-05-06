// Copyright Yerik Guo

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
    //在调用MakeEffectContext()创建Handle后，该指针的生命周期由Handle接管
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
