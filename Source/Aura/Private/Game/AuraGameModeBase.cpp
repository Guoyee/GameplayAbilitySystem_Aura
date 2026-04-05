// Copyright Yerik Guo


#include "Game/AuraGameModeBase.h"

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	if (UWorld* World = GetWorld())
	{
		World->GetWorldSettings()->SetTimeDilation(1.0f);
	}
}
