// Copyright Yerik Guo


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	BeginProjectileSpawn();
	
}

void UAuraProjectileSpell::BeginProjectileSpawn()
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	
	CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		// projectile 的Location和Rotation在FinishProjectileSpawn中更新
		
		check(ProjectileClass);
		DeferredProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass, 
			PendingSpawnTransform, 
			GetOwningActorFromActorInfo(), 
			Cast<APawn>(GetOwningActorFromActorInfo()), 
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		//TODO: 给弹体添加造成伤害的 Ganmeplay Effect
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		DeferredProjectile->DamageEffectSpecHandle = SpecHandle;
	}
}

void UAuraProjectileSpell::FinishProjectileSpawn(const FVector& ProjectileTargetLocation)
{   
	if (DeferredProjectile)
	{
		//在实际发射时刻，更新Location和Rotation信息
		FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.0f;//与地面的角度
		PendingSpawnTransform.SetLocation(SocketLocation);
		PendingSpawnTransform.SetRotation(Rotation.Quaternion());
		
		DeferredProjectile->FinishSpawning(PendingSpawnTransform);
		DeferredProjectile = nullptr;
	}
}
