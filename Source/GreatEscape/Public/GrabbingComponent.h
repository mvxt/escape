// MIT License - Copyright (c) 2018 Michael Thanh

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GrabbingComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GREATESCAPE_API UGrabbingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabbingComponent();

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
    // Player reach
    float Reach = 100.0f;
    // Physics handle component
    UPhysicsHandleComponent* PhysicsHandle = nullptr;
    // Input component
    UInputComponent* InputComponent = nullptr;

    // Find attached input component
    void BindInputActions();
    // Returns a hit for first physics body in reach
    FHitResult GetFirstHitInReach() const;
    // Find attached physics handle component
    void GetPhysicsHandleComponent();
    // Ray-cast and grab what's in reach
    void Grab();
    // Let go of object if something is grabbed
    void Release();
};
