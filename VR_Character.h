// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MotionControllerComponent.h"
#include "VR_Character.generated.h"


UCLASS()
class VR_AI_PROGRAMING_API AVR_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVR_Character();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void LocationTeleportDistance();
	void TeleportBegin();
	void TeleportFinish();
	bool FindTeleportDestination(TArray<FVector>& OutPath, FVector& OutLocation);
	void TeleportIndex();
	void StartFade(float FromAlpha, float ToAlpha);
	void UpdateBlinkers();
	void UpdateSpline(const TArray<FVector>& Path);
	void SnapRotation(float Local);
	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE FHitResult GetHitFire()const { return HitFire; }


private:

	
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float RotationRate = 0.0f;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SocketAnim", meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* RightMotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SocketAnim", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* RightHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SocketAnim", meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* LeftMotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SocketAnim", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* LeftHand;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DestinationMark;

	
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SocketAnim", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* body;

	
	UPROPERTY(EditAnywhere)
		float MaxTeleport = 1000.f;


	float TeleportProjectileRadius = 10;

	UPROPERTY(EditAnywhere)
		float TeleportProjectileSpeed = 800;

	UPROPERTY(EditAnywhere)
		float TeleportSimulationTime = 2;
	UPROPERTY(EditAnywhere)
		float Durationfadeout = 1.f;
	UPROPERTY(EditAnywhere)
		FVector TeleportationExtent = FVector(100.f, 100.f, 100.f);

	

	

	

	
};
