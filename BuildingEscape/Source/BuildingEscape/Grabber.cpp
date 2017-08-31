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

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber ready!"))
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player viewpoint this tick
	FVector location;
	FRotator rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT location, OUT rotation);

	//log
	//UE_LOG(LogTemp, Warning, TEXT("Viewport location: %s, rotation: %s"), *location.ToString(), *rotation.ToString());

	FVector lineTraceEnd = location + rotation.Vector()*reach;

	UKismetSystemLibrary::DrawDebugLine(GetWorld(), location, lineTraceEnd, FColor(255, 0, 0), false, 10.0);


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


}

