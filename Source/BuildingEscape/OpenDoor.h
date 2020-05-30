// Copyright Arnold Lam 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime); // returns true if successful
	void CloseDoor(float DeltaTime); // returns true if successful
	float GetTotalMassOfActors() const;
	void FindAudioComponent();

private:
	float InitialYaw;
	float CurrentYaw;

	UPROPERTY(EditAnywhere) float TargetYaw = -90.f;
	bool OpenDoorCalled = false;
	UPROPERTY(EditAnywhere) ATriggerVolume* PressurePlateRed = nullptr;
	UPROPERTY(EditAnywhere) ATriggerVolume* PressurePlateBlue = nullptr;
	UPROPERTY(EditAnywhere) ATriggerVolume* PressurePlateGreen = nullptr;
	UPROPERTY(EditAnywhere) float DoorOpenSpeed = 2.f;
	UPROPERTY(EditAnywhere) float DoorCloseSpeed = 5.f;

	float DoorLastOpened = 0.f;
	UPROPERTY(EditAnywhere) float DoorCloseDelay = .6f;
	UPROPERTY(EditAnywhere) float PressurePlateTriggerMinMass = 50.f;

	// Audio
	UPROPERTY() UAudioComponent* AudioComponent = nullptr;
	bool HasOpenSoundPlayed = false; // Tracks whether sound has been played
	bool HasCloseSoundPlayed = true;

	bool CheckIfAllBoxesAreCorrect();
};
