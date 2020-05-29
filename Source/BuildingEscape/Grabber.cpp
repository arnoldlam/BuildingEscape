// Copyright Arnold Lam 2020


#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	// Checking for Physics Handle Component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>(); // Common parent between Grabber and PhysicsHandle
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no PhysicsHandle component found"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent() 
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Input component found on actor %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("Input component not found on actor %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	FVector LineTraceEnd = GetLineTraceEnd();

	FHitResult PhysicsBodyToGrab = GetFirstPhysicsBodyInReach();

	if (PhysicsBodyToGrab.GetActor())
	{
		UPrimitiveComponent* ComponentToGrab = PhysicsBodyToGrab.GetComponent();
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None, 
			LineTraceEnd
		);
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

// Debug function to draw debug line
void UGrabber::EnableDrawDebugLine()
{
	DrawDebugLine(
		GetWorld(),
		GetPlayerLocation(),
		GetLineTraceEnd(),
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5.f
	);
}

FVector UGrabber::GetLineTraceEnd()
{
	// Get player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return (PlayerViewPointLocation + (Reach * PlayerViewPointRotation.Vector()));
}

FVector UGrabber::GetPlayerLocation()
{
	// Get player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

// Returns first physics body within the Reach distance
FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	FHitResult Hit;
	
	FCollisionQueryParams TraceParams(
		FName(TEXT("")), 
		false, GetOwner()
	);

	// Ray-cast out to a certain distance (Reach)
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerLocation(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If grabbing an object, move object to view point
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

