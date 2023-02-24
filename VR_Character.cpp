// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_Character.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h" 
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/EngineTypes.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"
#include "MyEnemyClass.h"
#include "Bullet_Hit_Interface.h"
#include "EnemyActor.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AVR_Character::AVR_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

	Player = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player"));
	Player->SetupAttachment(Camera);

	body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("body"));
	body->SetupAttachment(Camera);

	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
	RightMotionController->SetupAttachment(VRRoot);
	RightMotionController->SetTrackingSource(EControllerHand::Right);

	RightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(RightMotionController);

	

	DestinationMark = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestinationMark"));
	DestinationMark->SetupAttachment(VRRoot);

	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
	LeftMotionController->SetupAttachment(VRRoot);
	LeftMotionController->SetTrackingSource(EControllerHand::Left);

	LeftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(LeftMotionController);

	


	
}

// Called when the game starts or when spawned
void AVR_Character::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void AVR_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LocationTeleportDistance();
}
bool AVR_Character::FindTeleportDestination(TArray<FVector>& OutPath, FVector& OutLocation)
{
	FVector Look = RightMotionController->GetForwardVector();
	//Look = Look.RotateAngleAxis(30.f, RightController->GetRightVector());

	FVector Location = RightMotionController->GetComponentLocation() + Look * 15.0f;
	//location with axis and reach value
	//FVector End = Location + Look * MaxTeleport;
	//FHitResult TeleHit;

	FPredictProjectilePathParams Params(TeleportProjectileRadius, Location, Look * TeleportProjectileSpeed, TeleportSimulationTime, ECollisionChannel::ECC_MAX, this);
	Params.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	Params.bTraceComplex = true;

	FPredictProjectilePathResult Result;

	bool bSucess = UGameplayStatics::PredictProjectilePath(this, Params, Result);
	//GetWorld()->LineTraceSingleByChannel(TeleHit, Location, End, ECollisionChannel::ECC_Visibility);
	if (!bSucess) return false;
	//conveting into array of location
													//hitRest.pathdata which tooks the info of the every spins point
	for (FPredictProjectilePathPointData PointData : Result.PathData)
	{
		//FPredictProjectilePathPointData has its built variable Location,time,velocity i took location
		OutPath.Add(PointData.Location);
	};

	FNavLocation Navigation;

	bool bNavigative = UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation(Result.HitResult.Location, Navigation, TeleportationExtent);
	if (!bNavigative)return false;
	//we assign to argument of the function
	OutLocation = Navigation.Location;
	//this return is for outlocation = Navigation.Location;
	return true;

}
void AVR_Character::LocationTeleportDistance()
{
	//we replaced another variable which will give the same argument functionality
	TArray<FVector> Path;
	//no need of reference while passing the arugument
			 //Nav.location
	FVector Location;
	bool bHasDestination = FindTeleportDestination(Path, Location);
	if (bHasDestination)
	{

		DestinationMark->SetVisibility(true);
		//Nav.location
		DestinationMark->SetWorldLocation(Location);

	}
	else
	{
		DestinationMark->SetVisibility(false);

		//TArray<FVector> EmptyPath;
		//DrawTeleportaionPath(EmptyPath);
	}
}
// Called to bind functionality to input
void AVR_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AVR_Character::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &AVR_Character::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("snapYawRotation"), this, &AVR_Character::SnapRotation);
	PlayerInputComponent->BindAction(TEXT("Teleportation"), IE_Released, this, &AVR_Character::TeleportFinish);

}
void AVR_Character::MoveForward(float Value)
{
	AddMovementInput(Value * Camera->GetForwardVector());
}

void AVR_Character::MoveRight(float Value)
{
	AddMovementInput(Value * Camera->GetRightVector());
}
void AVR_Character::SnapRotation(float Local)
{

	AddControllerYawInput(Local * RotationRate * GetWorld()->GetDeltaSeconds());

}




void AVR_Character::TeleportFinish()
{
	FVector Destination = DestinationMark->GetComponentLocation();
	SetActorLocation(Destination);

}