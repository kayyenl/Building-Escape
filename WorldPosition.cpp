// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()                              //This code will run when the object is constructed.
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()  //code here happens when we press play.
{
	Super::BeginPlay();  //means that everything farther up the hierarchy has to be run first, before this code runs.

	// FString Log = TEXT("Hello!");
	// FString* PrtLog = &Log;  //The FString in this case is the type the pointer is POINTING TO. We are also AddressOf the Log!

	// Log.Len();

	// (*PrtLog).Len(); //We have dereferenced the PrtLog first in a bracket, because the dot operator would resolve earlier than *!
	// PrtLog -> Len();

	// UE_LOG(LogTemp, Warning, TEXT("%s"), **PrtLog);  //this asterisk is being overloaded to do something different.

	FString ObjectName = GetOwner()->GetName();
	FString ObjectPosition = GetOwner() -> GetActorLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("The location of %s is %s!"), *ObjectName, *ObjectPosition);
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// any code here will be run every frame of the game!
}

