#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridDecor.generated.h"

UCLASS()
class PROJECT_DUNE_API AGridDecor : public AActor
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

	AGridDecor();
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

public:

	int AssignedIndex;

};
