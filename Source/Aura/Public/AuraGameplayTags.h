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
    
    FGameplayTag Attributes_Primary_Strength;
    FGameplayTag Attributes_Primary_Intelligence;
    FGameplayTag Attributes_Primary_Resilience;
    FGameplayTag Attributes_Primary_Vigor;
    
    FGameplayTag Attributes_Secondary_Armor;
    FGameplayTag Attributes_Secondary_ArmorPenetration;
    FGameplayTag Attributes_Secondary_BlockChance;
    FGameplayTag Attributes_Secondary_CriticalHitChance;
    FGameplayTag Attributes_Secondary_CriticalHitDamage;
    FGameplayTag Attributes_Secondary_CriticalHitResistance;
    FGameplayTag Attributes_Secondary_HealthRegeneration;
    FGameplayTag Attributes_Secondary_ManaRegeneration;
    FGameplayTag Attributes_Secondary_MaxHealth;
    FGameplayTag Attributes_Secondary_MaxMana;
    FGameplayTag Attributes_Secondary_AttackSpeed;
    
    FGameplayTag InputTag_LMB;
    FGameplayTag InputTag_RMB;
    FGameplayTag InputTag_1;
    FGameplayTag InputTag_2;
    FGameplayTag InputTag_3;
    FGameplayTag InputTag_4;
    
    FGameplayTag Damage;
    FGameplayTag Damage_Fire;
    FGameplayTag Damage_Lightning;
    FGameplayTag Damage_Arcane;
    FGameplayTag Damage_Physical;

    TMap<FGameplayTag, FGameplayTag> DamageTypeToResistances;

    FGameplayTag Attributes_Resistance_Fire;
    FGameplayTag Attributes_Resistance_Lightning;
    FGameplayTag Attributes_Resistance_Arcane;
    FGameplayTag Attributes_Resistance_Physical;

    FGameplayTag Effects_HitReact;

    FGameplayTag Ability_Attack_Ranged;
    FGameplayTag Ability_Attack_Melee;
    FGameplayTag Cooldown_Attack;
    FGameplayTag Event_Montage_SpawnProjectile;
    
private:
    static FAuraGameplayTags SingletonGameplayTags;

    // [Claude] 禁止拷贝，保护单例不被复制
    FAuraGameplayTags() = default;
    FAuraGameplayTags(const FAuraGameplayTags&) = delete;
    FAuraGameplayTags& operator=(const FAuraGameplayTags&) = delete;
};
