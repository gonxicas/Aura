#include "Character/AuraEnemy.h"


AAuraEnemy::AAuraEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAuraEnemy::HighlightActor()
{
	bIsHighlighted = true;
}

void AAuraEnemy::UnHighlightActor()
{
	bIsHighlighted = false;
}

void AAuraEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!bIsHighlighted) return;
	
	DrawDebugSphere(GetWorld(), GetActorLocation(), 50.f, 10, FColor::Red, false, 0);
	
}
