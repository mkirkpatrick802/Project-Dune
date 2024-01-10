#include "GridManager.h"

#include "EngineUtils.h"
#include "GridDecor.h"
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
	DecorSpawnRate = .5f;

	ActorRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Actor Root"));
	ActorRootComponent->SetupAttachment(RootComponent);
	SetRootComponent(ActorRootComponent);

	InstancedTileComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Grid Tiles"));
	InstancedTileComponent->SetupAttachment(RootComponent);

	InstancedTileComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InstancedTileComponent->NumCustomDataFloats = 2;

	TileDecorInstances = CreateDefaultSubobject<USceneComponent>(TEXT("Tile Decor"));
	TileDecorInstances->SetupAttachment(RootComponent);

	OnLevelLoaderSpawned.AddUObject(this, &AGridManager::LevelLoaderSpawned);
}

void AGridManager::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < InstancedTileComponent->GetInstanceCount(); i++)
	{
		InstancedTileComponent->SetCustomDataValue(i, 1, 0, true);
	}

	for (int i = 0; i < InstancesDecorComponents.Num(); i++)
	{
		UInstancedStaticMeshComponent* Component = InstancesDecorComponents[i];
		if(!Component) continue;
		for (int j = 0; j < Component->GetInstanceCount(); j++)
		{
			Component->SetCustomDataValue(j, 0, 0, true);
		}
	}
}

void AGridManager::SetNumCustomDataFloats(UInstancedStaticMeshComponent* Component, int NumDataFloats)
{
	Component->NumCustomDataFloats = NumDataFloats;
}

void AGridManager::ShowDecorInstances()
{
	for (int i = 0; i < InstancesDecorComponents.Num(); i++)
	{
		UInstancedStaticMeshComponent* Component = InstancesDecorComponents[i];
		if (!Component) continue;
		for (int j = 0; j < Component->GetInstanceCount(); j++)
		{
			Component->SetCustomDataValue(j, 0, 1, true);
		}
	}
}

void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(TileActor && HasAuthority())
	{
		UpdateTiles();
	}
}

void AGridManager::LevelLoaderSpawned(ULevelLoaderComponent* LevelLoaderComponent)
{
	LevelLoaders.AddUnique(LevelLoaderComponent->AttachedActor);
	UE_LOG(LogTemp, Warning, TEXT("Level Loader Added!"))
}

void AGridManager::UpdateTiles()
{
	ActiveTiles.Empty();

	//Load active tiles
	for (const auto Loader : LevelLoaders)
	{
		const ULevelLoaderComponent* LevelLoaderComponent = Loader->GetComponentByClass<ULevelLoaderComponent>();
		if (!LevelLoaderComponent) continue;
		FVector Center = Loader->GetActorLocation();

		// Spawn Tiles
		TArray<int32> MeshIndices = InstancedTileComponent->GetInstancesOverlappingSphere(Center, LevelLoaderComponent->SphereRadius, true);
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

			// Copy Material Data
			FTransform CurrentInstanceTransform;
			InstancedTileComponent->GetInstanceTransform(Index, CurrentInstanceTransform, true);

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = this;

			AGridTile* Tile = GetWorld()->SpawnActor<AGridTile>(TileActor, CurrentInstanceTransform, SpawnParams);
			Tile->AssignedIndex = Index;
			Tile->Height = CurrentInstanceTransform.GetLocation().Z / TileHeightStrength - TileHeightOffset;
			Tile->UpdateMaterial();

			SpawnedTiles.AddUnique(Tile);
		}

		// Spawn Decor
		for (UInstancedStaticMeshComponent* DecorInstanceComponent : InstancesDecorComponents)
		{
			if (!DecorInstanceComponent) continue;
			for (int32 InstanceIndex = 0; InstanceIndex < DecorInstanceComponent->GetInstanceCount(); InstanceIndex++)
			{
				FTransform InstanceTransform;
				if (DecorInstanceComponent->GetInstanceTransform(InstanceIndex, InstanceTransform, true))
				{
					FVector InstanceLocation = InstanceTransform.GetLocation();
					if((InstanceLocation - Center).Size() <= LevelLoaderComponent->SphereRadius)
					{
						DecorInstanceComponent->SetCustomDataValue(InstanceIndex, 0, 1, true);
					}
				}
			}
		}
	}

	//Unload not active tiles
	for (int i = 0; i < SpawnedTiles.Num(); i++)
	{
		AGridTile* Tile = SpawnedTiles[i];
		if(ActiveTiles.Contains(Tile->AssignedIndex)) continue;

		SpawnedTiles.Remove(Tile);
		Tile->Destroy();
	}
}