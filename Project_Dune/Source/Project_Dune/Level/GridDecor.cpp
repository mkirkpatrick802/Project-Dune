#include "GridDecor.h"

AGridDecor::AGridDecor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	SetRootComponent(StaticMesh);
}

void AGridDecor::BeginPlay()
{
	Super::BeginPlay();
}

void AGridDecor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

