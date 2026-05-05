// Copyright Yerik Guo

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
    GENERATED_BODY()

public:
    AAuraCharacterBase();
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    UAttributeSet* GetAttributeSet() const {return AttributeSet;}
    
    virtual UAnimMontage* GetHitReactMontage_Implementation() override;
    
    /* Combat Interface*/
    virtual void Die() override;
    /* end Combat Interface*/
    
    UFUNCTION(NetMulticast, Reliable)
    virtual void MulticastHandleDeath();

    virtual void OnDeath() {}
protected:
    virtual void BeginPlay() override;
    
    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<USkeletalMeshComponent> Weapon;
    
    UPROPERTY(EditAnywhere, Category = "Combat")
    FName WeaponTipSocketName;
    
    virtual FVector GetCombatSocketLocation() override;
    
    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
    
    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;
    
    virtual void InitAbilityActorInfo();
    
    /*
     * 1.对于玩家操控角色，这些值在角色蓝图（BP_AuraCharacter）的details面板中配置
     * 2.对于怪物，通过怪物的 ECharacterClass 类别来匹配对应的DefaultPrimaryAttributes
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attribute")
    TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attribute")
    TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attribute")
    TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
    
    void ApplayEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
    
    void InitializeDefaultAttributes() const;
    
    void AddCharacterAbilities() const;
    
    /* Dissolve Effects */
    
    //将当前的材质替换为DissolveMaterialInstance
    void Dissolve();
    
    UFUNCTION(BlueprintImplementableEvent)
    void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

    UFUNCTION(BlueprintImplementableEvent)
    void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
private:
    UPROPERTY(EditAnywhere, Category = "Abilities")
    TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
    
    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<UAnimMontage> HitReactMontage;
};
