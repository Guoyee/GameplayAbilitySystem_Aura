// Copyright Yerik Guo

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "RangedAttackAbility.generated.h"

class ARangedProjectile;
class ICombatInterface;

UCLASS()
class AURA_API URangedAttackAbility : public UAuraDamageGameplayAbility
{
    GENERATED_BODY()

public:
    URangedAttackAbility();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
        FGameplayTagContainer* OptionalRelevantTags) const override;

    virtual bool ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* Payload) const override { return true; }

protected:
    // 子类/蓝图可覆写：获取攻击目标（默认从 TriggerEventData 中读取）
    virtual AActor* GetAttackTarget(const FGameplayEventData* TriggerEventData);

    // 子类可覆写：获取弹体生成位置
    virtual FVector GetSpawnLocation();

    UFUNCTION(BlueprintCallable)
    void SpawnProjectile(AActor* HomingTarget);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ARangedProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UAnimMontage* AttackMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float FlightSpeed = 3000.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float HitAcceptanceRadius = 50.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bTrackTarget = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cooldown")
    TSubclassOf<UGameplayEffect> AttackCooldownEffectClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float BaseAttackInterval = 1.0f;

    void ApplyAttackCooldown() const;

private:
    void BuildAndSpawnProjectile(AActor* Target);

    FGameplayEffectSpecHandle BuildDamageSpecHandle() const;
};
