// Copyright anamus

#pragma once

#include "BuildingEscape.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // Reach of the crabber
	float Reach = 100.f;
    
    UPhysicsHandleComponent *PhysicsHandle = nullptr;
    UInputComponent *DefaultInputComponent = nullptr;
    
    // Ray-cast into the distance, and crab what's in reach
    void Grab();
    void Release();
    
    void FindPhysicsHandleComponent();
    void SetupInputComponent();
    
    // Return the first hit physics body in reach
    FHitResult GetFirstPhysicsBodyInReach() const;
};
