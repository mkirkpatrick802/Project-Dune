#include "LevelLoaderComponent.h"

#include "EngineUtils.h"
#include "Level/GridManager.h"

ULevelLoaderComponent::ULevelLoaderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SphereRadius = 500;
}

void ULevelLoaderComponent::BeginPlay()
{
	Super::BeginPlay();

	AttachedActor = GetOwner();
	for (TActorIterator<AGridManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AGridManager* GridManager = *ActorItr;
		if (GridManager != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Adding Level Loader From: %s"), *AttachedActor->GetName());
			GridManager->OnLevelLoaderSpawned.Broadcast(this);
		}
	}
}

void ULevelLoaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}