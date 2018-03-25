// MIT License - Copyright (c) 2018 Michael Thanh

#include "Components/PrimitiveComponent.h"
#include "GrabbingComponent.h"
#include "Engine/World.h"

/*!
    Constructor, sets basic system-level parameters for the game engine.
*/
UGrabbingComponent::UGrabbingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  
    // You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

/*!
    Called when game starts. Configures various modules and other
    components
*/
void UGrabbingComponent::BeginPlay()
{
	Super::BeginPlay();

    // Look for attached PhysicsHandleComponent
    GetPhysicsHandleComponent();
    // Look for input component
    BindInputActions();
}

/*!
    Called every tick. See inline comments for functionality.
*/
void UGrabbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // TODO: Refactor, this is reused code
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );

    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

    // If a physics handle is attached, move the object we are holding.
    if (PhysicsHandle->GrabbedComponent) {
        PhysicsHandle->SetTargetLocation(LineTraceEnd);
    }
}

/*!
    Fetches the Input component to grab and release items. Binds to the
    "Grab" input and calls the appropriate methods. If none is found,
    exit game gracefully after printing error message to log.

    @see UGrabbingComponent::Grab()
    @see UGrabbingComponent::Release()
*/
void UGrabbingComponent::BindInputActions() {
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
        // Input Component found
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabbingComponent::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabbingComponent::Release);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No InputComponent found on %s"), *GetOwner()->GetName());
        FGenericPlatformMisc::RequestExit(false);
    }
}

/*!
Ray-casts outward using LineTrace and returns a hit to any actor
with a PhysicsBody collision channel set.

@return FHitResult
*/
FHitResult UGrabbingComponent::GetFirstHitInReach() const
{
    // Get player view point / where they are looking
    // Ray-cast out forward, see what object is hit
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );

    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
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

    return Hit;
}

/*!
    Fetches the Physics Component. If no physics handle component found, exit
    gracefully after printing error message to log.

    @see UGrabbingComponent::Grab()
    @see UGrabbingComponent::Release()
*/
void UGrabbingComponent::GetPhysicsHandleComponent() {
    // Look for attached PhysicsHandleComponent
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle)
    {
        // Physics Handle Component found
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No PhysicsHandleComponent found on %s"), *GetOwner()->GetName());
        FGenericPlatformMisc::RequestExit(false);
    }
}

/*!
    Attempts to pick up any object the player is looking at if withing
    reach.

    @see UGrabbingComponent::GetFirstHitInReach()
*/
void UGrabbingComponent::Grab() {

    // Raycast outward and return a hit if exists
    auto Hit = GetFirstHitInReach();

    // If we hit something, grab it by its ball. See following link:
    // https://community.gamedev.tv/t/grabcomponent-working-but-getting-error-with-componenttograb-fix/34753
    auto ActorHit = Hit.GetActor();
    if (ActorHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
        UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();
        PhysicsHandle->GrabComponent(
            ComponentToGrab,
            NAME_None,
            ActorHit->GetActorLocation(),
            true // Allow rotation
        );
    }
}

/*!
    If an object is currently being grabbed, when the button is released, the
    object will be released.
*/
void UGrabbingComponent::Release() {
    PhysicsHandle->ReleaseComponent();
}