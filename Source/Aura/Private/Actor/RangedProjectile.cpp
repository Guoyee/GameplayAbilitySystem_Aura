// Copyright Yerik Guo

#include "Actor/RangedProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ARangedProjectile::ARangedProjectile()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
    SetRootComponent(Sphere);
    Sphere->SetCollisionObjectType(ECC_Projectile);
    Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
    ProjectileMovement->InitialSpeed = 0.f;
    ProjectileMovement->MaxSpeed = 0.f;
    ProjectileMovement->ProjectileGravityScale = 0.f;
}

void ARangedProjectile::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(LifeSpan);
}

void ARangedProjectile::SetTarget(AActor* InTarget)
{
    TargetActor = InTarget;
}

void ARangedProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!HasAuthority()) return;

    AActor* Target = TargetActor;
    if (!IsValid(Target))
    {
        Destroy();
        return;
    }

    if (bTrackTarget)
    {
        const FVector Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        ProjectileMovement->Velocity = Direction * FlightSpeed;
    }

    const float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
    if (Distance <= HitAcceptanceRadius)
    {
        OnTargetReached();
    }
}

void ARangedProjectile::OnTargetReached()
{
    if (!HasAuthority()) return;

    AActor* Target = TargetActor;
    if (!IsValid(Target))
    {
        Destroy();
        return;
    }

    if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target))
    {
        TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
    }

    MulticastImpactEffect(GetActorLocation());
    Destroy();
}

void ARangedProjectile::MulticastImpactEffect_Implementation(const FVector& ImpactLocation)
{
    if (!HasAuthority() && bClientHitEffect) return;
    bClientHitEffect = true;

    UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, ImpactLocation, FRotator::ZeroRotator);
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, ImpactLocation);
}

void ARangedProjectile::Destroyed()
{
    Super::Destroyed();
}
