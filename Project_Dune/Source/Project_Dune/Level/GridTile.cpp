#include "GridTile.h"

AGridTile::AGridTile()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	SetRootComponent(StaticMesh);
}

void AGridTile::BeginPlay()
{
	Super::BeginPlay();
}

void AGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

