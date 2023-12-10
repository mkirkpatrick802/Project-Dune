#include "GridTile.h"

AGridTile::AGridTile()
{
	PrimaryActorTick.bCanEverTick = true;
	TileScale = 1;


	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Tile Mesh"));
	TileMesh->SetupAttachment(RootComponent);
	TileMesh->Mobility = EComponentMobility::Movable;
	TileMesh->SetWorldScale3D(FVector(TileScale, TileScale, TileScale));
	SetRootComponent(TileMesh);
}

void AGridTile::BeginPlay()
{
	Super::BeginPlay();
}

void AGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FTransform NewTransform;
	NewTransform.SetLocation(GetActorLocation() + FVector(0, 0, 1));
	SetActorTransform(NewTransform);
}