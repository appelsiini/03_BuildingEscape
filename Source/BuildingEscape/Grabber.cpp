// Copyright anamus

#include "BuildingEscape.h"
#include "Grabber.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    FindPhysicsHandleComponent();
    SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
    /// Check if there's a PhysicsHandle attached into the owner of this component
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle)
    {
        // Attached PhysicsHandle found
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Error in %s! Crabber won't work without a properly attached PhysicsHandle component, none found."),
               *GetOwner()->GetName());
    }
}

void UGrabber::SetupInputComponent()
{
    /// Check to make sure there's an input component available
    DefaultInputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (DefaultInputComponent)
    {
        /// Bind the input action
        DefaultInputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        DefaultInputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Error in %s! Crabber won't work without a properly attached InputComponent, none found."),
               *GetOwner()->GetName());
    }
}

void UGrabber::Grab()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab pressed."));
    
    /// Line-trace and see if we reach any actors with physics body collision channel set
    GetFirstPhysicsBodyInReach();
    
    /// If we hit something, then attach physics handle
    // TODO: Attach physics handle
}

void UGrabber::Release()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab released."));
    // TODO: Detach physics handle
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // If the physics handle is attached
        // Move the object that the actor is holding

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const {
    
    // Get the player's viewpoints details
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation
                                                               );
    
    
    // Draw a red line into world to visualise reach
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    
    // Setup query params
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    
    // Line-trace (aka. ray-cast) out into the distance
    FHitResult Hit;
    GetWorld()->LineTraceSingleByObjectType(
                                            OUT Hit,
                                            PlayerViewPointLocation,
                                            LineTraceEnd,
                                            // All pickable objects in this game will be inheriting PhysicsBody
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                            TraceParameters
                                            );
    
    // Log out the hit actor name
    AActor* ActorHit = Hit.GetActor();
    if (ActorHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Line-trace just hit %s"), *(ActorHit->GetName()));
    };
    
    return Hit;
}
