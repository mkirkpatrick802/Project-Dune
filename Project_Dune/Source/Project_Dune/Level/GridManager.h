#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

UCLASS()
class PROJECT_DUNE_API AGridManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

public:	

	AGridManager();
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

private:

	void UpdateTiles();

public:

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	int GridWidth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	int GridHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	float GridTileOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	float GridTileScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	bool IsInstancedStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tile, meta = (AllowPrivateAccess = "true"))
	UStaticMesh* TileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tile, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AGridTile> TileActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tile, meta = (AllowPrivateAccess = "true"))
	float TileHeightStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tile, meta = (AllowPrivateAccess = "true"))
	float TileHeightOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> LevelLoaders;

	UPROPERTY()
	TArray<int> ActiveTiles;

	UPROPERTY()
	TArray<AGridTile*> SpawnedTiles;

public:

};
