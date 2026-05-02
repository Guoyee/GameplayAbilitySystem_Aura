// Copyright Yerik Guo

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	//该技能的默认InputTag(默认键位), 在蓝图的details中设置 
	FGameplayTag StartupInputTag;
};
