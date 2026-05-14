// Copyright Yerik Guo


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    
    APawn* OwingPawn = AIOwner->GetPawn();

    const FName TargetTag = OwingPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");
    
    TArray<AActor*> ActorsWithTag;
    UGameplayStatics::GetAllActorsWithTag(OwingPawn, TargetTag, ActorsWithTag);
    //在TArray中寻找最近的目标
    
    float ClosestDistance = FLT_MAX;
    AActor* ClosestActor = nullptr;
    for (AActor* Actor : ActorsWithTag)
    {
        GEngine->AddOnScreenDebugMessage(2,.5f,FColor::Orange, *Actor->GetName());
        
        if (IsValid(Actor) && IsValid(OwingPawn))
        {
            const float Distance = OwingPawn->GetDistanceTo(Actor);
            if (Distance < ClosestDistance)
            {
                ClosestDistance = Distance;
                ClosestActor = Actor;
            }
        }
    }
    UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
    UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
}
