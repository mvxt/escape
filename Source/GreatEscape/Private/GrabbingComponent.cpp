// MIT License - Copyright (c) 2018 Michael Thanh

#include "GrabbingComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UGrabbingComponent::UGrabbingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  
    // You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabbingComponent::BeginPlay()
{
	Super::BeginPlay();

	// Look for attached PhysicsHandleComponent
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle)
    {
        // Physics Handle Component found
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No PhysicsHandleComponent found on %s"), *GetOwner()->GetName());
    }
}


// Called every frame
void UGrabbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view point / where they are looking
    // Ray-cast out forward, see what object is hit
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );

    // Log out to test
    //UE_LOG(LogTemp, Warning, TEXT("Location: %s, Position: %s"), 
    //    *PlayerViewPointLocation.ToString(),
    //    *PlayerViewPointRotation.ToString());

    // Draw red trace in world for visualization
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    DrawDebugLine(
        GetWorld(),
        PlayerViewPointLocation,
        LineTraceEnd,
        FColor(255, 0, 0),
        false,
        0.0f,
        0.0f,
        5.0f
    );

    FHitResult Hit;
    FCollisionQueryParams TraceParameters = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

    // Ray-cast out to reach distance
    GetWorld()->LineTraceSingleByObjectType(
        OUT Hit,
        PlayerViewPointLocation,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParameters
    );

    // See what we hit
    AActor* ActorHit = Hit.GetActor();
    if (ActorHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
    }
}

