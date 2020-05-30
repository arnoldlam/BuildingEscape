// Copyright Arnold Lam 2020


#include "OpenDoor.h"

#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	FindAudioComponent();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	if (!PressurePlateRed)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressure plate is set"), *GetOwner()->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CheckIfAllBoxesAreCorrect()) {
		OpenDoor(DeltaTime);
	}

	//if (GetTotalMassOfActors() > PressurePlateTriggerMinMass)
	//{
	//	OpenDoor(DeltaTime);
	//	DoorLastOpened = GetWorld()->GetTimeSeconds();
	//}
	//else if ((GetWorld()->GetTimeSeconds() - DoorLastOpened) > DoorCloseDelay) {
	//	CloseDoor(DeltaTime);
	//}
}

bool UOpenDoor::CheckIfAllBoxesAreCorrect() {
	TArray<AActor*> ListOfOverlappingActors;
	PressurePlateRed->GetOverlappingActors(OUT ListOfOverlappingActors);
	if (ListOfOverlappingActors.Num() > 0)
	{
		for (AActor* Actor : ListOfOverlappingActors)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Actor->GetName());
		}
	}
	return true;
}

void UOpenDoor::OpenDoor(float DeltaTime) 
{
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator NewRotation = GetOwner()->GetActorRotation();
	NewRotation.Yaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, DoorOpenSpeed);
	GetOwner()->SetActorRotation(NewRotation);

	if (AudioComponent && !HasOpenSoundPlayed) {
		AudioComponent->Play();
		HasOpenSoundPlayed = true;
		HasCloseSoundPlayed = false;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator NewRotation = GetOwner()->GetActorRotation();
	NewRotation.Yaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	GetOwner()->SetActorRotation(NewRotation);

	if (AudioComponent && !HasCloseSoundPlayed) {
		AudioComponent->Play();
		HasCloseSoundPlayed = true;
		HasOpenSoundPlayed = false;
	}
}

float UOpenDoor::GetTotalMassOfActors() const
{
	float TotalMass = 0;
	TArray<AActor*> ListOfOverlappingActors;

	PressurePlateRed->GetOverlappingActors(OUT ListOfOverlappingActors);
	if (ListOfOverlappingActors.Num() > 0) {
		for (AActor* Actor : ListOfOverlappingActors)
		{
			TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}
	
	return TotalMass;
}

void UOpenDoor::FindAudioComponent() 
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("No audio component found on actor %s"), *GetOwner()->GetName())
	}
}

