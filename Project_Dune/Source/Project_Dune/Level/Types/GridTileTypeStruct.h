#pragma once
#include "GridTileTypes.h"
#include "Engine/DataTable.h"
#include "GridTileTypeStruct.generated.h"

class AGridTile;

USTRUCT(BlueprintType)
struct FGridTileType : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	EGridTileTypes TileType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Tile;
};