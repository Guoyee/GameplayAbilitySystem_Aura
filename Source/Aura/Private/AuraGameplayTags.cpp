// Copyright Yerik Guo


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"
#include "../../../../../Program Files/Epic Games/UE_5.3/Engine/Plugins/Editor/GameplayTagsEditor/Source/GameplayTagsEditor/Private/GameplayTagEditorUtilities.h"

FAuraGameplayTags FAuraGameplayTags::SingletonGameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
    /* 
     * Primary Attributes 
     */
    SingletonGameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Strength"),
        FString("Increases physical damage")
    );
    
    SingletonGameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Intelligence"),
        FString("Increases magical damage")
    );
    
    SingletonGameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Resilience"),
        FString("Increases Armor and Armor Penetration")
    );
    
    SingletonGameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Vigor"),
        FString("Increases Health")
    );
    
    /* 
     * Secondary Attributes
     */
    SingletonGameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.Armor"),
        FString("Reduces damage taken, improves Block Chance")
    );
    
    SingletonGameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.ArmorPenetration"),
        FString("Ignores target's Armor")
    );
    
    SingletonGameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.BlockChance"),
        FString("Chance to block incoming attacks")
    );
    
    SingletonGameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.CriticalHitChance"),
        FString("Chance to deal critical damage")
    );
    
    SingletonGameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.CriticalHitDamage"),
        FString("Multiplier for critical hit damage")
    );
    
    SingletonGameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.CriticalHitResistance"),
        FString("Reduces chance of being critically hit")
    );
    
    SingletonGameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.HealthRegeneration"),
        FString("Health restored per second")
    );
    
    SingletonGameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.ManaRegeneration"),
        FString("Mana restored per second")
    );
    
    SingletonGameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.MaxHealth"),
        FString("Maximum Health amount")
    );
    
    SingletonGameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.MaxMana"),
        FString("Maximum Mana amount")
    );
    
    /* 
    * Input Tags
    */
    SingletonGameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("InputTag.LMB"),
    FString("Input Tag for Left Mouse Button")
    );
    
    SingletonGameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("InputTag.RMB"),
    FString("Input Tag for Right Mouse Button")
    );

    SingletonGameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.1"),
        FString("Input Tag for Key 1")
    );

    SingletonGameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.2"),
        FString("Input Tag for Key 2")
    );

    SingletonGameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.3"),
        FString("Input Tag for Key 3")
    );

    SingletonGameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.4"),
        FString("Input Tag for Key 4")
    );
    
    /*
    * Damage Types
    */
    SingletonGameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Damage"),
        FString("Damage")
    );

    SingletonGameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Damage.Fire"),
        FString("Fire Damage Type")
    );

    SingletonGameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Damage.Lightning"),
        FString("Lightning Damage Type")
    );

    SingletonGameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Damage.Arcane"),
        FString("Arcane Damage Type")
    );

    SingletonGameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Damage.Physical"),
        FString("Physical Damage Type")
    );

    /*
    * Resistance Attributes
    */
    SingletonGameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Resistance.Fire"),
        FString("Resistance to Fire damage")
    );

    SingletonGameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Resistance.Lightning"),
        FString("Resistance to Lightning damage")
    );

    SingletonGameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Resistance.Arcane"),
        FString("Resistance to Arcane damage")
    );

    SingletonGameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Resistance.Physical"),
        FString("Resistance to Physical damage")
    );

    /*
    * DamageType → Resistance Mappings
    */
    SingletonGameplayTags.DamageTypeToResistances.Add(
        SingletonGameplayTags.Damage_Fire,
        SingletonGameplayTags.Attributes_Resistance_Fire);

    SingletonGameplayTags.DamageTypeToResistances.Add(
        SingletonGameplayTags.Damage_Lightning,
        SingletonGameplayTags.Attributes_Resistance_Lightning);

    SingletonGameplayTags.DamageTypeToResistances.Add(
        SingletonGameplayTags.Damage_Arcane,
        SingletonGameplayTags.Attributes_Resistance_Arcane);

    SingletonGameplayTags.DamageTypeToResistances.Add(
        SingletonGameplayTags.Damage_Physical,
        SingletonGameplayTags.Attributes_Resistance_Physical);

    
    /*
    * Effects
    */
    SingletonGameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Effects.HitReact"),
        FString("HitReact")
    );
}
