// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

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
	owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	FRotator NewRotation = FRotator(0.0, openAngle, 0.0);
	owner->SetActorRotation(NewRotation);
}


void UOpenDoor::CloseDoor()
{
	FRotator NewRotation = FRotator(0.0, 0.f, 0.0);
	owner->SetActorRotation(NewRotation);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger 
	if (GetTotalMassOfActorsOnPlate() >= 40.f/*TriggerMass*/) { //TODO 
		// if the actor that opens is in the volume then open the door
		OpenDoor();
		lastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// check if it's time to close the door
	if (GetWorld()->GetTimeSeconds() - lastDoorOpenTime > doorClosedDelay) {
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float totalMass = 0.f;

	// find all overlapping actors
	TArray<AActor*> overlappingActors;
	pressurePlate->GetOverlappingActors(OUT overlappingActors);

	// iterate through them adding masses
	for (auto& actor : overlappingActors) {
		
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

		UE_LOG(LogTemp, Warning, TEXT("Overlapping actor: %s"), *actor->GetName());
	}

	return totalMass;
}


