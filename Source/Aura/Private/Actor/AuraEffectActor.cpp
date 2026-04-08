// Copyright Yerik Guo


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

void AAuraEffectActor::ApplayInfiniteEffectToTarget(AActor* TargetActor,TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass, EEffectRemovalPolicy InfiniteEffectRemovalPolicy)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;
	check(InfiniteGameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(InfiniteGameplayEffectClass, 1.f, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		AddActiveEffectHandle(TargetASC, ActiveEffectHandle);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	for (auto [GameplayEffectClass,ApplicationPolicy]:InstantGameplayEffectList)
	{
		if (ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
        	{
        		ApplyEffectToTarget(TargetActor, GameplayEffectClass);
        	}
	}
	
	for (auto [GameplayEffectClass,ApplicationPolicy]:DurationGameplayEffectList)
	{
		if (ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
    	{
    		ApplyEffectToTarget(TargetActor, GameplayEffectClass);
    	}	
	}

	for (auto [GameplayEffectClass,ApplicationPolicy,InfiniteEffectRemovalPolicy]:InfiniteGameplayEffectList)
	{
		if (ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplayInfiniteEffectToTarget(TargetActor, GameplayEffectClass, InfiniteEffectRemovalPolicy);
		}	
	}	
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	for (auto [GameplayEffectClass,ApplicationPolicy]:InstantGameplayEffectList)
	{
		if (ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffectClass);
		}
	}
	
	for (auto [GameplayEffectClass,ApplicationPolicy]:DurationGameplayEffectList)
	{
		if (ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffectClass);
		}	
	}

	for (auto [GameplayEffectClass,ApplicationPolicy,InfiniteEffectRemovalPolicy]:InfiniteGameplayEffectList)
	{
		if (ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplayInfiniteEffectToTarget(TargetActor, GameplayEffectClass, InfiniteEffectRemovalPolicy);
		}	
		
		if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
			if (!IsValid(TargetASC)) return;
		
			if (ActiveEffectHandles.Contains(TargetASC))
			{
				for (auto Handle : ActiveEffectHandles[TargetASC].Handles)
				{
					TargetASC->RemoveActiveGameplayEffect(Handle, 1);
				}
				ActiveEffectHandles.Remove(TargetASC);
			}
		}
	}
}

void AAuraEffectActor::AddActiveEffectHandle(UAbilitySystemComponent* AbilitySystem, const FActiveGameplayEffectHandle& Handle)
{
	if (!AbilitySystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddActiveEffectHandle: AbilitySystem is null!"));
		return;
	}
	// Get or add the struct in the map
	FActiveEffectHandleList& HandleList = ActiveEffectHandles.FindOrAdd(AbilitySystem);
	HandleList.Handles.Add(Handle);
}

