#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/GridTileTypeStruct.h"
#include "GridManager.generated.h"

UCLASS()
class PROJECT_DUNE_API AGridManager : public AActor
{
	GENERATED_BODY()

public:	

	AGridManager();
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

private:

	void UpdateTiles();
	bool IsPointInSphere(const FVector& Point, const FVector& SphereCenter, const float SphereRadius) const;
	bool IsBoxInSphere(const FVector& SphereCenter, const float SphereRadius, const FVector& BoxCenter, const FVector& BoxExtent);

public:

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	int GridSideLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	int GridTileOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid, meta = (AllowPrivateAccess = "true"))
	TArray<AGridTile*> SpawnedTiles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGridTile> BaseTile;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> LevelLoaders;

	UPROPERTY(VisibleAnywhere)
	TArray<AGridTile*> ActiveTiles;

public:

};
