#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTile.generated.h"

UCLASS()
class PROJECT_DUNE_API AGridTile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TileMesh;
	
public:	

	AGridTile();
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

private:

public:

private:

	float TileScale;

public:

	FORCEINLINE float GetTileScale() const { return TileScale; }

};
