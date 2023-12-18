#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

UCLASS()
class PROJECT_DUNE_API AGridManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ActorRootComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	UInstancedStaticMeshComponent* InstancedTileComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tile, meta = (AllowPrivateAccess = "true"))
	USceneComponent* TileDecorInstances;

public:	

	AGridManager();
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

private:

	void UpdateTiles();

public:

	// Grid Settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid)
	int GridWidth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid)
	int GridHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid)
	float GridTileOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid)
	float GridTileScale;

	// Tile Settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tile)
	UStaticMesh* TileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tile)
	TSubclassOf<class AGridTile> TileActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tile)
	float TileHeightStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tile)
	float TileHeightOffset;

	// Decoration Settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Decor)
	float DecorSpawnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Decor)
	TArray<UStaticMesh*> DecorationMeshes;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Decor)
	TArray<UInstancedStaticMeshComponent*> InstancesDecorComponents;

private:

	UPROPERTY()
	TArray<AActor*> LevelLoaders;

	UPROPERTY()
	TArray<int> ActiveTiles;

	UPROPERTY()
	TArray<AGridTile*> SpawnedTiles;

public:

};
