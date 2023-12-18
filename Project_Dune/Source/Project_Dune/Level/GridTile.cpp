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

	if(BaseMaterial)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		StaticMesh->SetMaterial(0, DynamicMaterialInstance);
	}
}

void AGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}