#include "LevelLoaderComponent.h"

#include "Components/SphereComponent.h"

ULevelLoaderComponent::ULevelLoaderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SphereRadius = 500;
}

void ULevelLoaderComponent::BeginPlay()
{
	Super::BeginPlay();

	AttachedActor = GetOwner();
    if (!AttachedActor) { UE_LOG(LogTemp, Error, TEXT("Level Loader Component is not attached to a valid actor.")); return; };

	AttachedActor->Tags.Add(FName("Level Loader"));
}

void ULevelLoaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}