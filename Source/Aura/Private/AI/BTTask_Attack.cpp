// Copyright Yerik Guo

#include "AI/BTTask_Attack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    APawn* ControlledPawn = AIController->GetPawn();
    if (!ControlledPawn) return EBTNodeResult::Failed;

    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlledPawn);
    if (!ASC) return EBTNodeResult::Failed;

    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(FName("TargetToFollow")));
    if (!TargetActor) return EBTNodeResult::Failed;

    FGameplayTagContainer AbilityTags;
    AbilityTags.AddTag(ActivationTag);

    TArray<FGameplayAbilitySpec*> MatchingAbilities;
    ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, MatchingAbilities, true);

    FGameplayEventData EventData;
    EventData.Target = TargetActor;

    for (FGameplayAbilitySpec* Spec : MatchingAbilities)
    {
        if (!Spec->IsActive())
        {
            ASC->TriggerAbilityFromGameplayEvent(Spec->Handle, ASC->AbilityActorInfo.Get(), ActivationTag, &EventData, *ASC);
            break;
        }
    }

    return Super::ExecuteTask(OwnerComp, NodeMemory);
}
