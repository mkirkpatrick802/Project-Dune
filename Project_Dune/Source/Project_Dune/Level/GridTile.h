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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;

	void UpdateMaterial();

protected:

	virtual void BeginPlay() override;

private:

public:	

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* BaseMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	int AssignedIndex;

	UPROPERTY(Replicated)
	float Height;

private:

public:

};
