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
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No UInputComponent attached to %s"), *GetOwner()->GetName())
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const {
	FTwoVectors ViewPointVectors = GetTraceVectors();
	const FVector StartTrace = ViewPointVectors.v1;
	const FVector EndTrace = ViewPointVectors.v2;
	FHitResult HitResult(ForceInit);
	FCollisionQueryParams TraceParameters = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult,	StartTrace, EndTrace, ECC_PhysicsBody,	TraceParameters);
	return HitResult;
}

FTwoVectors UGrabber::GetTraceVectors() const {
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT ViewPointLocation,
		OUT ViewPointRotation
	);
	const FVector StartTrace = ViewPointLocation;
	const FVector TraceDirection = ViewPointRotation.Vector();
	const FVector EndTrace = ViewPointLocation + TraceDirection * PlayerReach;
	return FTwoVectors(StartTrace, EndTrace);
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetTraceVectors().v2);
	}

	// if the physics handle is attached
		// move the object we're grabbing


	/// see what we hit that can be grabbed :D
}
