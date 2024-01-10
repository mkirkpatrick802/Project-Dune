#include "GridTile.h"

#include "Net/UnrealNetwork.h"

AGridTile::AGridTile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	SetRootComponent(StaticMesh);
}

void AGridTile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGridTile, Height)
}

void AGridTile::BeginPlay()
{
	Super::BeginPlay();

	if(BaseMaterial)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		StaticMesh->SetMaterial(0, DynamicMaterialInstance);
	}

	UpdateMaterial();
}

void AGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridTile::UpdateMaterial()
{
	if (DynamicMaterialInstance)
		DynamicMaterialInstance->SetScalarParameterValue(FName("Elevation"), Height);
}
