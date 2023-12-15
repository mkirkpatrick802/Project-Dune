#include "GridManager.h"

#include <algorithm>
#include <iterator>

#include "EngineUtils.h"
#include "GridTile.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Project_Dune/LevelLoaderComponent.h"

AGridManager::AGridManager()
{
	PrimaryActorTick.bCanEverTick = true;

	GridHeight = 2;
	GridWidth = 2;
	GridTileOffset = 10;
	GridTileScale = 1;
	IsInstancedStaticMesh = true;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMesh"));
	InstancedStaticMeshComponent->SetupAttachment(RootComponent);
	SetRootComponent(InstancedStaticMeshComponent);

	InstancedStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AGridManager::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < InstancedStaticMeshComponent->GetInstanceCount(); i++)
	{
		InstancedStaticMeshComponent->SetCustomDataValue(i, 1, 0, true);
	}
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

	}

	UpdateTiles();
}

void AGridManager::UpdateTiles()
{
	ActiveTiles.Empty();

	//Load active tiles
	for (const auto Loader : LevelLoaders)
	{
		const ULevelLoaderComponent* LevelLoaderComponent = Loader->GetComponentByClass<ULevelLoaderComponent>();
		if (!LevelLoaderComponent) break;

		FVector Center = Loader->GetActorLocation();
		TArray<int32> MeshIndices = InstancedStaticMeshComponent->GetInstancesOverlappingSphere(Center, LevelLoaderComponent->SphereRadius, true);
		for (auto Index : MeshIndices)
		{
			ActiveTiles.AddUnique(Index);

			bool IsSpawned = false;
			for (const AGridTile* Tile : SpawnedTiles)
				if(Tile->AssignedIndex == Index)
				{
					IsSpawned = true;
					break;
				}

			if(IsSpawned) continue;

			FTransform CurrentInstanceTransform;
			InstancedStaticMeshComponent->GetInstanceTransform(Index, CurrentInstanceTransform, true);

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = this;

			AGridTile* Tile = GetWorld()->SpawnActor<AGridTile>(TileActor, CurrentInstanceTransform, SpawnParams);
			Tile->AssignedIndex = Index;

			SpawnedTiles.AddUnique(Tile);
		}
	}

	//Unload not active tiles
	for (auto Tile : SpawnedTiles)
	{
		if(ActiveTiles.Contains(Tile->AssignedIndex)) continue;

		SpawnedTiles.Remove(Tile);
		Tile->Destroy();
	}
}