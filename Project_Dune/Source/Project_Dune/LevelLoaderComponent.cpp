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

    CreateDetectionSphere();

	AttachedActor->Tags.Add(FName("Level Loader"));
}

void ULevelLoaderComponent::CreateDetectionSphere()
{
	DetectionSphere = NewObject<USphereComponent>(AttachedActor, USphereComponent::StaticClass());

	if (DetectionSphere)
	{
		DetectionSphere->InitSphereRadius(SphereRadius);
		DetectionSphere->SetGenerateOverlapEvents(true);
		DetectionSphere->SetCollisionObjectType(ECC_WorldDynamic);
		DetectionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
		//DetectionSphere.

		DetectionSphere->AttachToComponent(AttachedActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		DetectionSphere->RegisterComponent();

		DetectionSphere->SetHiddenInGame(false);

		return;
	}

	UE_LOG(LogTemp, Error, TEXT("Failed to create Detection Sphere for owner actor %s"), *AttachedActor->GetName());
}

void ULevelLoaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}