// Copyright Yerik Guo

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 * 
 * Singleton containing native Gameplay Tags
 */

struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() {return SingletonGameplayTags;}
	static void InitializeNativeGameplayTags();
	
protected:
	
private:
	static FAuraGameplayTags SingletonGameplayTags;
};
