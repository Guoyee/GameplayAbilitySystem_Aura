// Copyright Yerik Guo

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_Attack.generated.h"

/**
 * 通用攻击任务 — 通过 GameplayTag 激活对应 Ability（远程/近战均可）
 */
UCLASS()
class AURA_API UBTTask_Attack : public UBTTask_BlueprintBase
{
    GENERATED_BODY()

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, Category = "Attack")
    FGameplayTag ActivationTag;
};
