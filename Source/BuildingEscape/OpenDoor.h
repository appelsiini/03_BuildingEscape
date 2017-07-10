// Copyright anamus

#pragma once

#include "Engine.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()
    
public:	
	// Sets default values for this component's properties
	UOpenDoor();
    
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Customs
    void OpenDoor();
    void CloseDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere)
    float OpenAngle = 0.f;
    
    float ClosedAngle = 90.f;
    
    UPROPERTY(EditAnywhere)
    ATriggerVolume* PressurePlate;

    UPROPERTY(EditAnywhere)
    float DoorAutoCloseDelay = 0.5f;
    
    float DoorLastOpened = 0.f;

    AActor* Owner;
    AActor* ActorThatOpens;
};
