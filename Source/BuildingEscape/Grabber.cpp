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
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToCrab = HitResult.GetComponent();
    auto ActorHit = HitResult.GetActor();
    
    /// If we hit something, then attach physics handle
    if (ActorHit)
    {
        // Attach physics handle
        // TODO: Upgrade to non-deprecated Grab method
        if (!PhysicsHandle) { return; }
        PhysicsHandle->GrabComponent(
                                     ComponentToCrab,
                                     NAME_None, // Not targeting bones in this case, therefor none
                                     ComponentToCrab->GetOwner()->GetActorLocation(), // Attach handle to base point, where x & y axis cross in the mesh
                                     true // Allow rotation
        );
    }
}

void UGrabber::Release()
{
    if (!PhysicsHandle) { return; }
    PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // If the physics handle is attached
    if (!PhysicsHandle) { return; }
    if (PhysicsHandle->GrabbedComponent)
    {
        // Move the object that the actor is holding, set new location vector to line-trace's location vector
        PhysicsHandle->SetTargetLocation(GetReachLineEnd());
    }
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
    // Setup query params
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    
    // Line-trace (aka. ray-cast) out into the distance
    FHitResult Hit;
    
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation
                                                               );
    
    GetWorld()->LineTraceSingleByObjectType(
                                            OUT Hit,
                                            PlayerViewPointLocation,
                                            GetReachLineEnd(),
                                            // All pickable objects in this game will be inheriting PhysicsBody
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                            TraceParameters
                                            );
    return Hit;
}

FVector UGrabber::GetReachLineEnd() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation
                                                               );
    return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
