#include "GridManager.h"
#include "GridTile.h"

AGridManager::AGridManager()
{
	PrimaryActorTick.bCanEverTick = false;

	GridLength = 10;
	GridWidth = 10;
}

void AGridManager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Delete old tiles
	for (const auto Tile : SpawnedTiles)
		if(Tile)
			Tile->Destroy();

	SpawnedTiles.Empty();

	// Spawn new tiles
	if (!GridTile) return;

	//float GridX = 0, GridY = 0;
	float TileScale = 1;
	FVector StartingLocation = GetActorLocation();
	const FRotator Rotation = GetActorRotation();

	for (int i = 0; i < GridWidth; i++)
	{
		for (int j = 0; j < GridLength; j++)
		{
			const FVector Location = StartingLocation + FVector(GridWidth * TileScale, GridLength * TileScale, 0);
			const FActorSpawnParameters SpawnInfo;
			AGridTile* SpawnedTile = (AGridTile*)GetWorld()->SpawnActor(GridTile, &Location, &Rotation, SpawnInfo);
			SpawnedTile->SetOwner(this);

			SpawnedTiles.AddUnique(SpawnedTile);
		}
	}
}

void AGridManager::BeginPlay()
{
	Super::BeginPlay();
}

void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

