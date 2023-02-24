// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"

#include "GameFramework/Actor.h"
#include "SplineActormovment.generated.h"

UCLASS()
class VR_AI_PROGRAMING_API ASplineActormovment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineActormovment();

	UFUNCTION(BlueprintCallable)
		void Play();//Play Function(to land the mesh)

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//spline point details about distance , location , rotation to place the mesh at the given Spline point index (FloorNumber)
	UFUNCTION()
		virtual void ProcessMovementTimeline(float Value);

	//End fuction 
	UFUNCTION()
		void OnEndMovementTimeline();

	
	UFUNCTION()
	 void AfterTheEnd();//optional //Unwanted


public:

	//component

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComponent;

	
	//Curvefloat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
		UCurveFloat* MovementCurve;

	//spline point     where to land (FloorNumber)
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		int32 FloorNumber;


	//UnWanted (or) //Optional to  Reverse or Restart the time line at the End 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline", meta = (EditCondition = "!bRestartOnEndTimeline"))
		bool bReverseOnEndTimeline;																							//flipflop between Reverse and Restart conditions

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline", meta = (EditCondition = "!bReverseOnEndTimeline"))
		bool bRestartOnEndTimeline;

protected:
	
	//SplineLocation and SplineRotation
	FVector CurrentSplineLocation;
	FRotator CurrentSplineRotation;

private:
	//Timeline
	FTimeline MovementTimeline;
};
