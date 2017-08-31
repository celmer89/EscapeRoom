// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
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
	FindPhysicsComponent();
	SetupInputComponent();
}

void UGrabber::FindPhysicsComponent()
{
	// Look or attached Physics Handle;
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsHandle) {
		UE_LOG(LogTemp, Warning, TEXT("Physics handle component found"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found: %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	// Look or attached input component
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input component found"));

		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No input component found: %s"), *GetOwner()->GetName());
	}
}


void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab!"));
	auto hit = GetFirstPhysicsBodyInReach();
	auto componentToGrab = hit.GetComponent();
	auto actorHit = hit.GetActor();
	//HandleComp->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), ComponentToGrab->GetOwner()->GetActorRotation());
	if (actorHit) {
		physicsHandle->GrabComponent(componentToGrab, NAME_None, componentToGrab->GetOwner()->GetActorLocation(), true);

	}

	// Line trace and try and reach any actors with physics body collisins channel set
	// if we hit soething then attach a physics
	// TODO attach physics handle
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Release!"));
	physicsHandle->ReleaseComponent();
	// TODO release physics
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (physicsHandle->GrabbedComponent) {

		FVector location;
		FRotator rotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT location, OUT rotation);

		FVector lineTraceEnd = location + rotation.Vector()*reach;

		physicsHandle->SetTargetLocation(lineTraceEnd);
	}
}



FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector location;
	FRotator rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT location, OUT rotation);

	FVector lineTraceEnd = location + rotation.Vector()*reach;

	//UKismetSystemLibrary::DrawDebugLine(GetWorld(), location, lineTraceEnd, FColor(255, 0, 0), false, 10.0);

	/// Ray-cast out to reach distance
	FHitResult hitResult;
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(OUT hitResult,
		location,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
	);
	// see what we hit
	auto actor = hitResult.GetActor();
	if (actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *actor->GetName());
	}


	return hitResult;
	//return FHitResult();
}

