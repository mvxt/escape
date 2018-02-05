// MIT License - Copyright (c) 2018 Michael Thanh

#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.
    // You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
}

void UOpenDoor::OpenDoor()
{
    // Get the owning actor to access rotation
    auto Door = GetOwner();

    // Create a rotator
    FRotator NewRotation = FRotator(0.0f, -60.0f, 0.0f);

    // Set new Rotator
    Door->SetActorRotation(NewRotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // Poll trigger volume.
    // If OpenActor is in volume, open the door
    if (PressurePlate->IsOverlappingActor(OpenActor))
    {
        OpenDoor();
    }
}

