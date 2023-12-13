#include "GridManager.h"

#include "EngineUtils.h"
#include "GridTile.h"
#include "Project_Dune/LevelLoaderComponent.h"

AGridManager::AGridManager()
{
	PrimaryActorTick.bCanEverTick = true;
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

	if (LevelLoaders.IsEmpty())
	{
		for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (ActorItr->Tags.Contains(FName("Level Loader")))
			{
				LevelLoaders.AddUnique(*ActorItr);
			}
		}

		UpdateTiles();
	}
}

void AGridManager::UpdateTiles()
{
	ActiveTiles.Empty();

	for (const auto Tile : SpawnedTiles)
	{
		for (const auto LevelLoader : LevelLoaders)
		{
			if (const ULevelLoaderComponent* LevelLoaderComponent = LevelLoader->GetComponentByClass<ULevelLoaderComponent>())
			{
				if (IsBoxInSphere(LevelLoader->GetActorLocation(), LevelLoaderComponent->SphereRadius, Tile->GetActorLocation(), Tile->GetBoxExtent()))
				{
					ActiveTiles.AddUnique(Tile);
				}
			}
		}
	}

	for (const auto Tile : ActiveTiles)
	{
		if(Tile->GetIsLoading()) continue;

		Tile->LoadIn();
	}

	for (const auto Tile : SpawnedTiles)
	{
		if (!ActiveTiles.Contains(Tile))
		{
			if (Tile->GetIsLoading()) continue;

			Tile->LoadOut();
		}
	}
}

bool AGridManager::IsPointInSphere(const FVector& Point, const FVector& SphereCenter, const float SphereRadius) const
{
	const float SquaredDistance = FVector::DistSquared(Point, SphereCenter);
	return SquaredDistance <= FMath::Square(SphereRadius);
}

bool AGridManager::IsBoxInSphere(const FVector& SphereCenter, const float SphereRadius, const FVector& BoxCenter, const FVector& BoxExtent)
{
	FVector ClosestPoint = FVector(
		FMath::Clamp(SphereCenter.X, BoxCenter.X - BoxExtent.X, BoxCenter.X + BoxExtent.X),
		FMath::Clamp(SphereCenter.Y, BoxCenter.Y - BoxExtent.Y, BoxCenter.Y + BoxExtent.Y),
		FMath::Clamp(SphereCenter.Z, BoxCenter.Z - BoxExtent.Z, BoxCenter.Z + BoxExtent.Z)
	);

	return FVector::DistSquared(ClosestPoint, SphereCenter) <= FMath::Square(SphereRadius);
}