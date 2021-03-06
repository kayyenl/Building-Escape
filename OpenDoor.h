// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioCompo();
	void FindPressurePlate();

	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

private:
	float CurrentYaw;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoors = 40.f;

	UPROPERTY(EditAnywhere)
	float DelayForDoorOpen = 0.5f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 2.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 2.0f;	

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;
	
	float InitialYaw;
	FRotator CurrentRotation;

	float DoorLastOpened = 0.f;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY()
	UAudioComponent* AudioCompo = nullptr;

};
