// Copyright Arnold Lam 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Reach = 100.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr; // Protects from random crashes
	UInputComponent* InputComponent = nullptr;

	void EnableDrawDebugLine();
	FVector GetLineTraceEnd();
	void SetupInputComponent();
	void FindPhysicsHandle();
	void Grab();
	void Release();
	FHitResult GetFirstPhysicsBodyInReach();
	FVector GetPlayerLocation();
};
