// Fill out your copyright notice in the Description page of Project Settings.

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

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner() -> GetActorRotation().Yaw;
	OpenAngle += InitialYaw;
	CurrentYaw = InitialYaw;

	FindPressurePlate();
	FindAudioCompo();
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)  //Good way to protect from nullpointer errors
	{
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor component, but it has no pressureplate set!"), *GetOwner() -> GetName());
	}
}

void UOpenDoor::FindAudioCompo() 
{
	AudioCompo = GetOwner() -> FindComponentByClass<UAudioComponent>();
	if (!AudioCompo)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have audio component!"), *GetOwner() -> GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpenDoors)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld() -> GetTimeSeconds();
	}
	else
	{
		if ((GetWorld() -> GetTimeSeconds()) - DoorLastOpened > DelayForDoorOpen)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	// FRotator CurrentRotation = GetOwner() -> GetActorRotation(); //cannot be called in the constructor, as it may return null as it is no owner is there yet as of initialisation.
	// UE_LOG(LogTemp, Warning, TEXT("The Actor's rotation is %s"), *CurrentRotation.ToString());
	// UE_LOG(LogTemp, Warning, TEXT("The current Yaw is %f"), CurrentRotation.Yaw);
	
	CurrentRotation = GetOwner() -> GetActorRotation();
	CurrentYaw = CurrentRotation.Yaw;
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed );
	CurrentRotation.Yaw = CurrentYaw;
	GetOwner() -> SetActorRotation(CurrentRotation);

	CloseDoorSound = false;
	if (!AudioCompo) {return;}
	if (!OpenDoorSound)
	{
		AudioCompo -> Play();
		OpenDoorSound = true;
		UE_LOG(LogTemp, Error, TEXT("Door Opens on %s"), *GetOwner() -> GetName());
	}

	// float MyFloat = 90.f;  //10 is a integer, 10.0 is a double, and 10.0f is a float. 10.f is fine too.

	// CurrentRotation.Yaw = MyFloat;
	// GetOwner() -> SetActorRotation(CurrentRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentRotation = GetOwner() -> GetActorRotation();
	CurrentYaw = CurrentRotation.Yaw;
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed );
	CurrentRotation.Yaw = CurrentYaw;
	GetOwner() -> SetActorRotation(CurrentRotation);

	OpenDoorSound = false;
	if (!AudioCompo) {return;}
	if (!CloseDoorSound)
	{
		AudioCompo -> Play();
		CloseDoorSound = true;
		UE_LOG(LogTemp, Error, TEXT("Door Closes on %s"), *GetOwner() -> GetName());
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f; 

	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) {return TotalMass;}
	PressurePlate -> GetOverlappingActors(OUT OverlappingActors);

	for (AActor* EachActor: OverlappingActors)
	{
		TotalMass += EachActor -> FindComponentByClass<UPrimitiveComponent>() -> GetMass();
	}
	return TotalMass;
}
