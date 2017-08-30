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

	//OpenDoor();
	actorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	//actorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	// ...
	
}

void UOpenDoor::OpenDoor()
{
	AActor* owner = GetOwner();

	//UWorld::GetWorld();


	FRotator NewRotation = FRotator(0.0, -90, 0.0);

	owner->SetActorRotation(NewRotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger 
	if (pressurePlate->IsOverlappingActor(actorThatOpens)) {
		// if the actor that opens is in the volume then open the door
		OpenDoor();
	}



}

