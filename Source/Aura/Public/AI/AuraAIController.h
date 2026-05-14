// Copyright Yerik Guo

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AuraAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    
    AAuraAIController();
    
protected:
    
    TObjectPtr<UBlackboardComponent> BlackboardComponent;
    
    TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
