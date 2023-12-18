#include "GridManager.h"

#include "EngineUtils.h"
#include "GridDecor.h"
#include "GridTile.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
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
	ActiveDecor.Empty();

	//Load active tiles
	for (const auto Loader : LevelLoaders)
	{
		const ULevelLoaderComponent* LevelLoaderComponent = Loader->GetComponentByClass<ULevelLoaderComponent>();
		if (!LevelLoaderComponent) break;
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

			float RelativeHeight = CurrentInstanceTransform.GetLocation().Z / TileHeightStrength - TileHeightOffset;

			if(Tile->DynamicMaterialInstance)
				Tile->DynamicMaterialInstance->SetScalarParameterValue(FName("Elevation"), RelativeHeight);

			UE_LOG(LogTemp, Warning, TEXT("%f"), RelativeHeight)

			SpawnedTiles.AddUnique(Tile);
		}

		// Spawn Decor
		for (UInstancedStaticMeshComponent* DecorInstanceComponents : InstancesDecorComponents)
		{
			TArray<int32> DecorIndices = DecorInstanceComponents->GetInstancesOverlappingSphere(Center, LevelLoaderComponent->SphereRadius, true);
			for (int32 DecorIndex : DecorIndices)
			{
				ActiveDecor.AddUnique(DecorIndex);

				bool IsSpawned = false;
				for (const AGridDecor* Decor : SpawnedDecor)
					if(Decor->AssignedIndex == DecorIndex)
					{
						IsSpawned = true;
						break;
					}

				if (IsSpawned) continue;

				FTransform CurrentInstanceTransform;
				DecorInstanceComponents->GetInstanceTransform(DecorIndex, CurrentInstanceTransform, true);

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = this;

				AGridDecor* Decor = GetWorld()->SpawnActor<AGridDecor>(AGridDecor::StaticClass(), CurrentInstanceTransform, SpawnParams);
				Decor->AssignedIndex = DecorIndex;
				Decor->StaticMesh->SetStaticMesh(DecorInstanceComponents->GetStaticMesh());

				SpawnedDecor.AddUnique(Decor);
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

	//Unload not active decor
	for (int i = 0; i < SpawnedDecor.Num(); i++)
	{
		AGridDecor* Decor = SpawnedDecor[i];
		if(ActiveDecor.Contains(Decor->AssignedIndex)) continue;

		SpawnedDecor.Remove(Decor);
		Decor->Destroy();
	}
}