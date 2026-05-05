// Copyright Yerik Guo


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
    
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
    for (const TSubclassOf<UGameplayAbility> Ability : Abilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
        //对于玩家技能，绑定一个默认快捷键
        if (const UAuraGameplayAbility* AuraGameplayAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
        {
            AbilitySpec.DynamicAbilityTags.AddTag(AuraGameplayAbility->StartupInputTag);
        }
        GiveAbility(AbilitySpec);
    }    
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(FGameplayTag& InputTag)
{
    if (!InputTag.IsValid()) return;
    
    for (FGameplayAbilitySpec& AbilitySpec: GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            AbilitySpecInputPressed(AbilitySpec);
            if (!AbilitySpec.IsActive())
            {
                TryActivateAbility(AbilitySpec.Handle);
            }
        }
    }
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(FGameplayTag& InputTag)
{
    if (!InputTag.IsValid()) return;
    
    for (FGameplayAbilitySpec& AbilitySpec: GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            AbilitySpecInputReleased(AbilitySpec);
        }
    }
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) const
{
    FGameplayTagContainer TagContainer;
    EffectSpec.GetAllAssetTags(TagContainer);
    
    EffectAssetTags.Broadcast(TagContainer);
    
}
