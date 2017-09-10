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
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner();

	if (!pressurePlate) {
		UE_LOG(LogTemp, Warning, TEXT("Missing pressure plate %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger 
	if (GetTotalMassOfActorsOnPlate() >= triggerMass) { //TODO 
		// if the actor that opens is in the volume then open the door
		FOnOpen.Broadcast();
	}
	else {
		FOnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float totalMass = 0.f;

	// find all overlapping actors
	if (!pressurePlate) {
		return totalMass;
	}
	TArray<AActor*> overlappingActors;
	pressurePlate->GetOverlappingActors(OUT overlappingActors);

	// iterate through them adding masses
	for (auto& actor : overlappingActors) {
		
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

		UE_LOG(LogTemp, Warning, TEXT("Overlapping actor: %s"), *actor->GetName());
	}

	return totalMass;
}


