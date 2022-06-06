// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//UE_LOG(LogTemp, Warning, TEXT("Hello, I am grabber"));
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetupInputComponent();
	
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) {return;}
	if (PhysicsHandle -> GrabbedComponent)
	{
		PhysicsHandle -> SetTargetLocation(GetLine());
	}
	//See if the physics handle is attached
		//Set the targets location according to LineTraceEnd
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner() -> FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) //This is for if the component is found. (originally PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Null pointer error in %s, please point to an actor!"), *(GetOwner() -> GetName()));
	}
}

void UGrabber::SetupInputComponent()
{
	InputCompo = GetOwner() -> FindComponentByClass<UInputComponent>();
	if (InputCompo)
	{
		UE_LOG(LogTemp, Error, TEXT("This Actor has a Input Component!"));
		InputCompo -> BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputCompo -> BindAction("Release", IE_Released, this, &UGrabber::Released);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("This Actor does not have an input component!"));
	}
}

void UGrabber::Grab()
{
	//UE_LOG(LogTemp, Error, TEXT("Grab function is working!"));

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* GrabbedComponentThing = HitResult.GetComponent();
	
	AActor* ActorHit = HitResult.GetActor();

	if (ActorHit)
	{	
		if (!PhysicsHandle) {return;}  //can use this to protect against null pointers too
		PhysicsHandle -> GrabComponentAtLocation(GrabbedComponentThing , NAME_None , GetLine());
	}
}

void UGrabber::Released()
{
	//UE_LOG(LogTemp, Error, TEXT("Release function is working!"));

	if (!PhysicsHandle) {return;}
	if (PhysicsHandle -> GrabbedComponent)
	PhysicsHandle -> ReleaseComponent();
	//Try to release/remove the physics handle.
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner() );
	// Ray-cast out to a certain distance

	GetWorld() -> LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerPosn(),
		GetLine(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// See what it hits
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("You have collided into %s!"), *(ActorHit -> GetName())); 
	}
	return Hit;
}

FVector UGrabber::GetLine() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
	OUT PlayerViewPointLocation,
	OUT PlayerViewPointRotation
	);
	
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * reach;
}

FVector UGrabber::GetPlayerPosn() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
	OUT PlayerViewPointLocation,
	OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}
// UE_LOG(LogTemp, Warning, TEXT("The current view location is %s!"), *OUT PlayerViewPointLocation.ToString());
// UE_LOG(LogTemp, Warning, TEXT("The current view rotation is %s!"), *OUT PlayerViewPointRotation.ToString());
// GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	//Draw a line from player showing the reach. (This is for debug line)
	// FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * reach;
	// DrawDebugLine(
	// 	GetWorld(),
	// 	PlayerViewPointLocation,
	// 	LineTraceEnd,
	// 	FColor(0, 255, 0),
	// 	false,
	// 	0.f,
	// 	0,
	// 	5.f
	// );
