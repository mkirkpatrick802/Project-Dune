#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/GridTileTypes.h"
#include "Types/GridTileTypeStruct.h"
#include "GridTile.generated.h"

UCLASS()
class PROJECT_DUNE_API AGridTile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Tile)
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = Tile)
	UChildActorComponent* TileActorComponent;

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

	void SetChildActorHidden(bool IsHidden);
	void SetChildActorLocation(const FVector& Location);

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:



private:

	UPROPERTY(EditAnywhere, Category = Tiles, meta = (AllowPrivateAccess = "true"))
	UDataTable* GridTileTypes;

	TMap<EGridTileTypes, FGridTileType> TileTypeMaps;
	EGridTileTypes TileType;

	UPROPERTY()
	AActor* TileActor;

	FVector DefaultPosition;
	FVector TargetPosition;
	bool IsLoaded;
	bool IsLoading;

public:

	FORCEINLINE bool GetIsLoaded() const { return IsLoaded; }
	FORCEINLINE bool GetIsLoading() const { return IsLoading; }

	FVector GetBoxExtent() const;

};
