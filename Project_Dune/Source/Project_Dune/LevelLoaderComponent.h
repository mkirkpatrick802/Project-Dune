// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelLoaderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_DUNE_API ULevelLoaderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	ULevelLoaderComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

private:

	void CreateDetectionSphere();

public:

	UPROPERTY(EditAnywhere, Category = LevelLoading)
	float SphereRadius;

	UPROPERTY()
	AActor* AttachedActor;
private:

public:

};
