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

void UOpenDoor::OpenDoor()
{
    if (!Owner) { return; }
//    Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
    OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor()
{
    if (!Owner) { return; }
//    Owner->SetActorRotation(FRotator(0.f, ClosedAngle, 0.f));
    OnCloseRequest.Broadcast();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Check if ActorThatOpens is overlapping with the PressurePlate, and open the door if true 
    if(GetTotalMassOfActorsOnPlate() > PressurePlateMassThreshold)
    {
        OpenDoor();
        DoorLastOpened = GetWorld()->GetTimeSeconds();
    }
    
    // Autoclose door after set threshold
    if (GetWorld()->GetTimeSeconds() > DoorLastOpened + DoorAutoCloseDelay)
    {
        CloseDoor();
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
