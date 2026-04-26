// Copyright Yerik Guo


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

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
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		
		PendingSpawnTransform.SetLocation(SocketLocation);
		//TODO: 设置 projectile 的朝向
		
		check(ProjectileClass);
		DeferredProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass, 
			PendingSpawnTransform, 
			GetOwningActorFromActorInfo(), 
			Cast<APawn>(GetOwningActorFromActorInfo()), 
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		//TODO: 给弹体添加造成伤害的 Ganmeplay Effect
	}
}

void UAuraProjectileSpell::FinishProjectileSpawn()
{   
	if (DeferredProjectile)
	{
		//在实际发射时刻，更新位置信息
		PendingSpawnTransform.SetLocation(CombatInterface->GetCombatSocketLocation());
		DeferredProjectile->FinishSpawning(PendingSpawnTransform);
		DeferredProjectile = nullptr;
	}
		
}
