// Copyright anamus

#include "OpenDoor.h"
#include "Gameframework/Actor.h"


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
    
    ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
    AActor* Owner = GetOwner();
    FRotator NewRotation = FRotator(0.f, 0.f, 0.f);
    
    Owner->SetActorRotation(NewRotation);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Check if ActorThatOpens is overlapping with the PressurePlate, and open the door if true 
    if(PressurePlate->IsOverlappingActor(ActorThatOpens))
    {
        OpenDoor();
    }
}

