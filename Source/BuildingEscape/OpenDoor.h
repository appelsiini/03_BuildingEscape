// Copyright anamus

#pragma once

#include "BuildingEscape.h"
#include "Engine.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
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

private:
    UPROPERTY(VisibleAnywhere)
    float OpenAngle = 0.f;
    
    float ClosedAngle = 90.f;
    
    UPROPERTY(EditAnywhere)
    ATriggerVolume* PressurePlate = nullptr;
    
    // Threshold mass in kilograms that trigger the door opening method
    UPROPERTY(EditAnywhere)
    float PressurePlateMassThreshold = 50.f;

    AActor* Owner = nullptr;
    
    // Get total mass in kilograms
    float GetTotalMassOfActorsOnPlate();
    
    UPROPERTY(BlueprintAssignable)
    FOnOpenRequest OnOpenRequest;
    
    UPROPERTY(BlueprintAssignable)
    FOnCloseRequest OnCloseRequest;
};
