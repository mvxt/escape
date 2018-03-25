// MIT License - Copyright (c) 2018 Michael Thanh

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GREATESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    // Opens the doors
    void OpenDoor();

    // Closes the doors
    void CloseDoor();

private:
    UPROPERTY(VisibleAnywhere)
    float OpenAngle = -80.0f;

    UPROPERTY(EditAnywhere)
    ATriggerVolume* PressurePlate;

    UPROPERTY(EditAnywhere)
    float DoorCloseDelay = 1.f;

    float LastDoorOpenTime;

    AActor* Owner;
    AActor* OpenActor;
};
