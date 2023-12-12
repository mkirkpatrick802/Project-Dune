#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/GridTileTypes.h"
#include "Types/GridTileTypeStruct.h"
#include "GridTile.generated.h"

class UTimelineComponent;

UCLASS()
class PROJECT_DUNE_API AGridTile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Tile)
	UChildActorComponent* TileActor;

public:	

	AGridTile();
	virtual void Tick(float DeltaTime) override;

	void LoadIn();
	void LoadOut();

	UFUNCTION(BlueprintCallable)
	FVector2D GetTileBounds();

protected:

	UFUNCTION(BlueprintCallable, Category = Tile)
	void PopulateMap();

	UFUNCTION(BlueprintCallable, Category = Tile)
	void SetChildActor();

	virtual void BeginPlay() override;

private:

public:



private:

	UPROPERTY(EditAnywhere, Category = Tiles, meta = (AllowPrivateAccess = "true"))
	UDataTable* GridTileTypes;

	TMap<EGridTileTypes, FGridTileType> TileTypeMaps;
	EGridTileTypes TileType;

	FVector DefaultPosition;
	FVector TargetPosition;
	bool IsLoaded;
	bool IsLoading;

public:

	FORCEINLINE bool GetIsLoaded() const { return IsLoaded; }
	FORCEINLINE bool GetIsLoading() const { return IsLoading; }

};
