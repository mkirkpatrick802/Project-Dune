#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

UCLASS()
class PROJECT_DUNE_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	

	AGridManager();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

private:

public:

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid, meta = (AllowPrivateAccess = "true"))
	int GridWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid, meta = (AllowPrivateAccess = "true"))
	int GridLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AGridTile> GridTile;

	TArray<AGridTile*> SpawnedTiles;

public:

};
