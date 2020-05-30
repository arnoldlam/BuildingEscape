// Copyright Arnold Lam 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "ButonForDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UButonForDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UButonForDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere) ATriggerVolume* TriggerVolume = nullptr;
	bool CheckIfButtonPressed();
	float ButtonPressedZDelta = 8.f;
	UPROPERTY(EditAnywhere) float ButtonPressSpeed = 1.5f;
	float InitialButtonZ;
	float TargetButtonZ;
	void PressButton(float DeltaTime);
	void LiftButton(float DeltaTime);
};
