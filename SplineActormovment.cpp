// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineActormovment.h"

// Sets default values
ASplineActormovment::ASplineActormovment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	SplineComponent->SetupAttachment(GetRootComponent());

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(SplineComponent);

	
}

// Called when the game starts or when spawned
void ASplineActormovment::BeginPlay()
{
	Super::BeginPlay();
	
	

	
}

// Called every frame
void ASplineActormovment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementTimeline.IsPlaying())
	{
		MovementTimeline.TickTimeline(DeltaTime);//Timeline functions every frame while playing

		
	}

}

void ASplineActormovment::ProcessMovementTimeline(float Value)
{

	const float SplineLength = SplineComponent->GetDistanceAlongSplineAtSplinePoint(FloorNumber); //Get the distance along the spline at the spline point( Distance between floor )

	CurrentSplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);//Given a distance along the length of this spline, return the point in space where this puts you ( corresponding Location to land )
	CurrentSplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);//Given a distance along the length of this spline, return a rotation corresponding to the spline's rotation there.( corresponding Rotation to land )

	CurrentSplineRotation.Pitch = 0.f;//to be aware of other axis
	CurrentSplineRotation.Yaw = 0.f;

	MeshComponent->SetWorldLocationAndRotation(CurrentSplineLocation, CurrentSplineRotation);//to place the mesh

}

void ASplineActormovment::OnEndMovementTimeline()
{
	MovementTimeline.Stop();
}

void ASplineActormovment::Play()
{
	
	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, TEXT("ProcessMovementTimeline"));//bind the fumction ProcessMovementTimeline 
	MovementTimeline.AddInterpFloat(MovementCurve, ProgressFunction);//Add floatcurve

	
	FOnTimelineEvent OnTimelineFinishedFunction;
	OnTimelineFinishedFunction.BindUFunction(this, TEXT("OnEndMovementTimeline"));// bind the fumction  OnEndMovementTimeline
	MovementTimeline.SetTimelineFinishedFunc(OnTimelineFinishedFunction);

	MovementTimeline.SetTimelineLengthMode(TL_LastKeyFrame);

	MovementTimeline.Play();

	
}

//UnWanted (or) //Optional to  Reverse or Restart the time line at the End 
void ASplineActormovment::AfterTheEnd()//Optional //Unwanted
{
	if (!MovementTimeline.IsReversing())
	{
		MovementTimeline.Reverse();
	}
}

