// Copyright Yerik Guo

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

enum class ECharacterClass : uint8;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
    GENERATED_BODY()
public:
    AAuraEnemy();
    
    /* Enemy Interface*/
    virtual void HighlightActor() override;
    virtual void UnHighlightActor() override;
    /* end Enemy Interface*/
    
    /* Combat Interface*/
    virtual int32 GetPlayerLevel() override;
    virtual void Die() override;
    /* end Combat Interface*/
    virtual void OnDeath() override;
    
    UPROPERTY(BlueprintAssignable)
    FOnAttributesChangedSignature OnHealthChanged;
    
    UPROPERTY(BlueprintAssignable)
    FOnAttributesChangedSignature OnMaxHealthChanged;
    
    void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
    
    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    bool bHitReacting;
    
    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    float BaseWalkSpeed = 300.f;
    
    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    float LifeSpan = 5.f;
protected:
    virtual void BeginPlay() override;
    
    virtual void InitAbilityActorInfo() override;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
    int32 Level = 1;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Class Defaults")
    ECharacterClass CharacterClass = ECharacterClass::None;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UWidgetComponent> HealthBar;
    
private:


};
