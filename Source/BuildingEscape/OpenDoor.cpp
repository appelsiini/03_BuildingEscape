// Copyright anamus

#include "BuildingEscape.h"
#include "OpenDoor.h"
#include "Gameframework/Actor.h"

#define OUT

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
    Owner = GetOwner();
    
    if (!PressurePlate)
    {
        UE_LOG(LogTemp, Error, TEXT("%s missing attached PressurePlate, won't work properly without."), *GetOwner()->GetName());
    }
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Check if ActorThatOpens is overlapping with the PressurePlate, and open the door if true 
    if(GetTotalMassOfActorsOnPlate() > PressurePlateMassThreshold)
    {
        OnOpenRequest.Broadcast();
    }
    else
    {
        OnCloseRequest.Broadcast();
    }
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
    
    TArray<AActor*> OverlappingActors;
    PressurePlate->GetOverlappingActors(OUT OverlappingActors);
    
    float TotalMass = 0.f;
    
    for (auto &Actor : OverlappingActors)
    {
        if (!Actor) { return TotalMass; }
        TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
    }
    
    return TotalMass;
}
