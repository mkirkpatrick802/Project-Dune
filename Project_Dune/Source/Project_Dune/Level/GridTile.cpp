#include "GridTile.h"

#include "Components/BoxComponent.h"

AGridTile::AGridTile()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Box Collider"));
	BoxComponent->SetupAttachment(RootComponent);
	SetRootComponent(BoxComponent);

	BoxComponent->Mobility = EComponentMobility::Stationary;
	BoxComponent->SetCollisionObjectType(ECC_WorldStatic);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	BoxComponent->SetGenerateOverlapEvents(true);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AGridTile::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AGridTile::OnBoxEndOverlap);

	TileActorComponent = CreateDefaultSubobject<UChildActorComponent>(FName("Tile Actor"));
	TileActorComponent->SetupAttachment(RootComponent);

	TileActorComponent->Mobility = EComponentMobility::Movable;
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

		TileActorComponent->SetChildActorClass(TileTypeMaps.Find(RandomEnumValue.GetValue())->Tile);
		TileActor = TileActorComponent->GetChildActor();
		if (!TileActor)
		{
			UE_LOG(LogTemp, Error, TEXT("Tile Actor Not Set"));
		}
	}
}

FVector2D AGridTile::GetTileBounds()
{
	if(TileActor)
	{
		FVector Origin;
		FVector Bounds;
		TileActor->GetActorBounds(true, Origin, Bounds);
		UE_LOG(LogTemp, Warning, TEXT("%f, %f"), Bounds.X, Bounds.Y);
		return FVector2D(Bounds.X * 2, Bounds.Y * 2);
	}

	return FVector2D::ZeroVector;
}

void AGridTile::BeginPlay()
{
	Super::BeginPlay();

	TileActor = TileActorComponent->GetChildActor();
	if (!TileActor)
	{
		UE_LOG(LogTemp, Error, TEXT("Tile Actor Not Set"));
	}

	DefaultPosition = GetActorLocation();
	TargetPosition = FVector::Zero();
	IsLoaded = false;
	IsLoading = false;

	SetChildActorHidden(true);
}

void AGridTile::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(IsLoading || !OtherActor->Tags.Contains(FName("Level Loader"))) return;

	LoadIn();
}

void AGridTile::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsLoading || !OtherActor->Tags.Contains(FName("Level Loader"))) return;

	LoadOut();
}

void AGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(TargetPosition == FVector::Zero()) return;
	if(TargetPosition == DefaultPosition)
	{
		const FVector NewPosition = FMath::Lerp(TileActor->GetActorLocation(), TargetPosition, .02f);
		SetChildActorLocation(NewPosition);

		const float DistanceSquared = FVector::DistSquared(TileActor->GetActorLocation(), DefaultPosition);
		const float ToleranceSquared = FMath::Square(50.f);
		if (DistanceSquared <= ToleranceSquared)
		{
			IsLoaded = true;
			IsLoading = false;
		}
	}
	else
	{
		const FVector NewPosition = FMath::Lerp(TileActor->GetActorLocation(), TargetPosition, .02f);
		SetChildActorLocation(NewPosition);

		const float DistanceSquared = FVector::DistSquared(TileActor->GetActorLocation(), TargetPosition);
		const float ToleranceSquared = FMath::Square(50.f);
		if (DistanceSquared <= ToleranceSquared)
		{
			TargetPosition = FVector::Zero();
			SetChildActorHidden(true);
			IsLoading = false;
		}
	}

	TArray<AActor*> OverlappingActors;
	int LevelLoaderCounter = 0;
	BoxComponent->GetOverlappingActors(OverlappingActors);
	for (const AActor* OverlappingActor : OverlappingActors)
		if(OverlappingActor->Tags.Contains(FName("Level Loader")))
			LevelLoaderCounter++;

	if (LevelLoaderCounter == 0)
		LoadOut();
}

void AGridTile::LoadIn()
{
	if(IsLoaded) return;
	IsLoading = true;

	TargetPosition = DefaultPosition;

	const FVector NewPosition = FVector(DefaultPosition.X, DefaultPosition.Y, DefaultPosition.Z + 200);
	SetChildActorLocation(NewPosition);

	SetChildActorHidden(false);
}

void AGridTile::LoadOut()
{
	if (!IsLoaded) return;
	IsLoading = true;

	const FVector NewPosition = FVector(DefaultPosition.X, DefaultPosition.Y, DefaultPosition.Z - 200);
	TargetPosition = NewPosition;

	IsLoaded = false;
}

void AGridTile::SetChildActorHidden(const bool IsHidden)
{
	if (!TileActor) return;

	TileActor->SetActorHiddenInGame(IsHidden);
}

void AGridTile::SetChildActorLocation(const FVector& Location)
{
	if (!TileActor) return;

	TileActor->SetActorLocation(Location);
}

FVector AGridTile::GetBoxExtent() const
{
	return BoxComponent->GetScaledBoxExtent();
}