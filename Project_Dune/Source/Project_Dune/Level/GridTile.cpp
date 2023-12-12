#include "GridTile.h"

AGridTile::AGridTile()
{
	PrimaryActorTick.bCanEverTick = true;

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

	DefaultPosition = GetActorLocation();
	TargetPosition = FVector::Zero();
	IsLoaded = false;
	IsLoading = false;
	SetActorHiddenInGame(!IsLoaded);
}

void AGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(TargetPosition == FVector::Zero()) return;
	if(TargetPosition == DefaultPosition)
	{
		const FVector NewPosition = FMath::Lerp(GetActorTransform().GetLocation(), TargetPosition, .03f);
		SetActorLocation(NewPosition);

		const float DistanceSquared = FVector::DistSquared(GetActorLocation(), DefaultPosition);
		const float ToleranceSquared = FMath::Square(10.f);
		if (DistanceSquared <= ToleranceSquared)
		{
			IsLoaded = true;
			IsLoading = false;
		}
	}
	else
	{
		const FVector NewPosition = FMath::Lerp(GetActorTransform().GetLocation(), TargetPosition, .03f);
		SetActorLocation(NewPosition);
		IsLoaded = false;

		const float ADistanceSquared = FVector::DistSquared(GetActorLocation(), TargetPosition);
		const float AToleranceSquared = FMath::Square(50.f);
		if (ADistanceSquared <= AToleranceSquared)
		{
			TargetPosition = FVector::Zero();
			SetActorHiddenInGame(true);
			IsLoading = false;
		}
	}
}

void AGridTile::LoadIn()
{
	if(IsLoaded) return;
	IsLoading = true;

	UE_LOG(LogTemp, Warning, TEXT("Loaded"));

	TargetPosition = DefaultPosition;

	const FVector NewPosition = FVector(DefaultPosition.X, DefaultPosition.Y, DefaultPosition.Z + 200);
	SetActorLocation(NewPosition);


	SetActorHiddenInGame(false);
}

void AGridTile::LoadOut()
{
	if (!IsLoaded) return;
	IsLoading = true;

	const FVector NewPosition = FVector(DefaultPosition.X, DefaultPosition.Y, DefaultPosition.Z - 200);
	TargetPosition = NewPosition;

	IsLoaded = false;
}