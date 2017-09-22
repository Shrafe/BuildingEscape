// Copyright Tom Hauser 2016

#include "BuildingEscape.h"
#include "Grabber.h"


#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupAttachedInputComponent();
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed"))

		/// line-trace any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit) {
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab key released"))
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("No UPhysicsHandleComponent attached to %s"), *GetOwner()->GetName())
	}
}

void UGrabber::SetupAttachedInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UInputComponent attached and found for %s"), *GetOwner()->GetName())
			InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No UInputComponent attached to %s"), *GetOwner()->GetName())
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	/// Get player viewpoint this tick
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT ViewPointLocation,
		OUT ViewPointRotation
	);

	/// Draw a red test trace to visualize in world
	FVector LineTraceEnd = ViewPointLocation + ViewPointRotation.Vector() * PlayerReach;

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

	return Hit;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent) {
		/// Get player viewpoint this tick
		FVector ViewPointLocation;
		FRotator ViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT ViewPointLocation,
			OUT ViewPointRotation
		);

		/// Draw a red test trace to visualize in world
		FVector LineTraceEnd = ViewPointLocation + ViewPointRotation.Vector() * PlayerReach;
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

	// if the physics handle is attached
		// move the object we're grabbing


	/// see what we hit that can be grabbed :D
}

