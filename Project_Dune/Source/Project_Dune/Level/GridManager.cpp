#include "GridManager.h"

AGridManager::AGridManager()
{
	PrimaryActorTick.bCanEverTick = false;
	GridSideLength = 10;
	GridTileOffset = 10;
}

void AGridManager::BeginPlay()
{
	Super::BeginPlay();
}

void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}