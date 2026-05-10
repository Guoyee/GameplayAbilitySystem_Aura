// Copyright Yerik Guo

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class ICombatInterface;
class AAuraProjectile;
/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
    GENERATED_BODY()
    
protected:
    
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    //让蓝图接管弹体的生成具体时间
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void BeginProjectileSpawn();
    
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void FinishProjectileSpawn(const FVector& ProjectileTargetLocation);
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly);
    TSubclassOf<AAuraProjectile> ProjectileClass;

    
private:
    // 存储已经做了Deferred Spawn，但尚未Finish的弹体
    UPROPERTY()
    AAuraProjectile* DeferredProjectile;
 
    // 记录SpawnTransform
    FTransform PendingSpawnTransform;
    
    //缓存接口变量用于获取socket位置
    ICombatInterface* CombatInterface;
    


};
