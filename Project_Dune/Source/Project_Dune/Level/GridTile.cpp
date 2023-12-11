#include "GridTile.h"

AGridTile::AGridTile()
{
	PrimaryActorTick.bCanEverTick = false;

	TileActor = CreateDefaultSubobject<UChildActorComponent>(FName("Tile Actor"));
	TileActor->SetupAttachment(RootComponent);
	TileActor->Mobility = EComponentMobility::Movable;
	SetRootComponent(TileActor);
}

void AGridTile::PopulateMap()
{
	if (GridTileTypes)
	{
		TArray<FName> GridTileNames = GridTileTypes->GetRowNames();
		for (auto RowName : GridTileNames)
		{
			FGridTileType GridTile = *GridTileTypes->FindRow<FGridTileType>(RowName, "");
			TileTypeMaps.Add(GridTile.TileType, GridTile);
		}
	}
}

void AGridTile::SetChildActor()
{
	if (GridTileTypes)
	{
		const int MaxEnumValue = (int)EGridTileTypes::GTT_Max;

		int RandomIndex = FMath::RandRange(0, MaxEnumValue - 1);
		const TEnumAsByte RandomEnumValue(static_cast<EGridTileTypes>(RandomIndex));

		TileActor->SetChildActorClass(TileTypeMaps.Find(RandomEnumValue.GetValue())->Tile);
	}
}

FVector2D AGridTile::GetTileBounds()
{
	AActor* ChildActor = TileActor->GetChildActor();
	if(ChildActor)
	{
		FVector Origin;
		FVector Bounds;
		ChildActor->GetActorBounds(true, Origin, Bounds);
		UE_LOG(LogTemp, Warning, TEXT("%f, %f"), Bounds.X, Bounds.Y);
		return FVector2D(Bounds.X * 2, Bounds.Y * 2);
	}

	return FVector2D::ZeroVector;
}

void AGridTile::BeginPlay()
{
	Super::BeginPlay();
}

void AGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}