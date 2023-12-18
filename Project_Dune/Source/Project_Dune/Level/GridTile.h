#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTile.generated.h"

UCLASS()
class PROJECT_DUNE_API AGridTile : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	AGridTile();
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* BaseMaterial;

	UMaterialInstanceDynamic* DynamicMaterialInstance;

	int AssignedIndex;

private:

public:

};
