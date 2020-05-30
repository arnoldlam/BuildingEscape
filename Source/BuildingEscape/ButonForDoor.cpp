// Copyright Arnold Lam 2020


#include "ButonForDoor.h"

#define OUT

// Sets default values for this component's properties
UButonForDoor::UButonForDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UButonForDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialButtonZ = GetOwner()->GetActorLocation().Z;
	TargetButtonZ = InitialButtonZ - ButtonPressedZDelta;
}


// Called every frame
void UButonForDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CheckIfButtonPressed())
	{
		PressButton(DeltaTime);
	}
	else 
	{
		LiftButton(DeltaTime);
	}
}

void UButonForDoor::PressButton(float DeltaTime) 
{
	FVector CurrentButtonLocation = GetOwner()->GetActorLocation();
	FVector NewButtonLocation = CurrentButtonLocation;

	// Animate button presss
	NewButtonLocation.Z = FMath::FInterpTo(CurrentButtonLocation.Z, TargetButtonZ, DeltaTime, ButtonPressSpeed);
	GetOwner()->SetActorLocation(NewButtonLocation);
}

void UButonForDoor::LiftButton(float DeltaTime)
{
	FVector CurrentButtonLocation = GetOwner()->GetActorLocation();
	FVector NewButtonLocation = CurrentButtonLocation;

	// Animate button presss
	NewButtonLocation.Z = FMath::FInterpTo(CurrentButtonLocation.Z, InitialButtonZ, DeltaTime, ButtonPressSpeed);
	GetOwner()->SetActorLocation(NewButtonLocation);
}

bool UButonForDoor::CheckIfButtonPressed()
{
	TArray<AActor*> ListOfOverlappingActors;

	TriggerVolume->GetOverlappingActors(OUT ListOfOverlappingActors);
	if (ListOfOverlappingActors.Num() > 0) {
		return true;
	}
	else 
	{
		return false;
	}
}