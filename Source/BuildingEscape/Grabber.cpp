// Copyright Tom Hauser 2016

#include "BuildingEscape.h"
#include "Grabber.h"


#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	// ...
	
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	/// Get player viewpoint this tick
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT ViewPointLocation, 
		OUT ViewPointRotation
	);
	
	/// Draw a red test trace to visualize in world
	FVector LineTraceEnd = ViewPointLocation + ViewPointRotation.Vector() * PlayerReach;

	DrawDebugLine(
		GetWorld(),
		ViewPointLocation,
		LineTraceEnd,
		FColor(0, 0, 255),
		false,
		0.f,
		0.f,
		10.f
	);

	/// Setup query parameters
	FCollisionQueryParams TraceParameters = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	/// Line-trace (ray cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		ViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line-trace first hit object: %s"), *(ActorHit->GetName()))
	}
	// see what we hit that can be grabbed :D
}

