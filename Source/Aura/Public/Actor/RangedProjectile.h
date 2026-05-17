// Copyright Yerik Guo

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "RangedProjectile.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class AURA_API ARangedProjectile : public AActor
{
    GENERATED_BODY()

public:
    ARangedProjectile();

    UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
    FGameplayEffectSpecHandle DamageEffectSpecHandle;

    UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
    float FlightSpeed = 3000.f;

    UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
    float HitAcceptanceRadius = 50.f;

    UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
    bool bTrackTarget = true;

    void SetTarget(AActor* InTarget);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void Destroyed() override;

    UFUNCTION(NetMulticast, Reliable)
    void MulticastImpactEffect(const FVector& ImpactLocation);

    void OnTargetReached();

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> Sphere;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UNiagaraSystem> ImpactEffect;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<USoundBase> ImpactSound;

    UPROPERTY(EditDefaultsOnly)
    float LifeSpan = 10.f;

private:
    UPROPERTY()
    TObjectPtr<AActor> TargetActor;

    bool bClientHitEffect = false;
};
