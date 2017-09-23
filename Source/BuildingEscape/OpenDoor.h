// Copyright Tom Hauser 2016

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor();
	void CloseDoor();



private:

	float OpenAngle;
	float CloseAngle;

	UPROPERTY(EditAnywhere)
    float DoorCloseDelay = 1.f;

	float LastDoorOpenTime;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float TriggerThreshold = 30.f;

	float GetTotalMassOfActorsOnPlate();

	AActor* Owner = nullptr;
};
