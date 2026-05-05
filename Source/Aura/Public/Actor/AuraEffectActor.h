// Copyright Yerik Guo

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
class USphereComponent;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
    ApplyOnOverlap,
    ApplyOnEndOverlap,
    DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
    RemoveOnEndOverlap,
    DoNotRemove
};

// 用于 Instant 和 Duration 效果的配置（无移除策略）
USTRUCT(BlueprintType)
struct FEffectWithConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> GameplayEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EEffectApplicationPolicy ApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
};

USTRUCT(BlueprintType)
struct FInfiniteEffectWithConfig
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> GameplayEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EEffectApplicationPolicy ApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
};

//用于ActiveEffectHandles的value
USTRUCT(BlueprintType)
struct FActiveEffectHandleList
{
    GENERATED_BODY()
    // 将数组放在结构体内部
    UPROPERTY(BlueprintReadOnly)
    TArray<FActiveGameplayEffectHandle> Handles;
};


UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
    GENERATED_BODY()
    
public:    
    AAuraEffectActor();


protected:
    virtual void BeginPlay() override;
    
    UFUNCTION(BlueprintCallable)
    void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);
    
    void ApplayInfiniteEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass,EEffectRemovalPolicy InfiniteEffectRemovalPolicy);
    
    UFUNCTION(BlueprintCallable)
    void OnOverlap(AActor* TargetActor);
    
    UFUNCTION(BlueprintCallable)
    void OnEndOverlap(AActor* TargetActor);
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    bool bDestroyOnEffectRemoval = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    TArray<FEffectWithConfig> InstantGameplayEffectList;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    TArray<FEffectWithConfig> DurationGameplayEffectList;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    TArray<FInfiniteEffectWithConfig> InfiniteGameplayEffectList;

    
    UPROPERTY()
    TMap<TObjectPtr<UAbilitySystemComponent>, FActiveEffectHandleList> ActiveEffectHandles;
    // Helper function for adding a handle:
    void AddActiveEffectHandle(UAbilitySystemComponent* AbilitySystem, const FActiveGameplayEffectHandle& Handle);
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    float ActorLevel = 1.f;

};
