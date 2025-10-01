// Proyecto 1 de motores graficos II. S, A y E.


#include "GameModeCollectable.h"
#include "Kismet/GameplayStatics.h"
#include "TargetHolder.h"
#include "Engine/TargetPoint.h"

AGameModeCollectable::AGameModeCollectable()
{
}

void AGameModeCollectable::BeginPlay()
{
	Super::BeginPlay();

	// Check if item class was specified.
	if (!ItemClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR: ITEM CLASS WAS NOT SPECIFIED FOR GAME MODE: %s"), *GetNameSafe(this));
		return;
	}

	TArray<AActor*> OutActors;

	// Find the Target Holder
	UGameplayStatics::GetAllActorsOfClass(
		this,
		ATargetHolder::StaticClass(),
		OutActors
	);

	// Check if no Target Holder was found.
	if (OutActors.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR: No Target holder was found FOR GAME MODE: %s"), *GetNameSafe(this));
		return;
	}
	// Save a reference to them (if found)

	// En teoria deberia haber solo una instancia de target holer en el nivel.
	// Por eso esta codificado especificamente para usar [0]
	if (ATargetHolder* Th = Cast<ATargetHolder>(OutActors[0]))
	{
		TargetHolder = Th;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("ERROR: Target Holder is null in %s"), *GetNameSafe(this));
	}

	// Spawn item
	if (UWorld* const World = GetWorld())
	{
		Item = World->SpawnActor<AActor>(ItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}
}


void AGameModeCollectable::PlaceInRandomPositionInRange(UPrimitiveComponent* Collider)
{
	if (TargetHolder->Points.IsEmpty())
	{
		UE_LOG(LogTemp, Display, TEXT("No target points found"));
		return;
	}
	// Temporarily disable collision
	Collider->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	// Place in a random location, preventing it from being the previous location.
	int32 PossibleLocation;
	do
	{
		PossibleLocation = FMath::RandRange(0, TargetHolder->Points.Num() - 1);
		Item->SetActorLocation(TargetHolder->Points[PossibleLocation]->GetActorLocation());
#if WITH_EDITOR
		UKismetSystemLibrary::PrintString(this,TEXT("Changed place!"));
#endif
	}
	while (LastItemLocation == PossibleLocation);
	LastItemLocation = PossibleLocation;
	Collider->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
}

void AGameModeCollectable::CallPlayerOverlappedWithItemDelegate(UPrimitiveComponent* Collider)
{
	// Change position of item
	PlaceInRandomPositionInRange(Collider);
	NumberOfItemsTaken++;
	UE_LOG(LogTemp, Display, TEXT("Number of items taken = %d. Called from %s"), NumberOfItemsTaken,
	       *GetNameSafe(this));

	OnPlayerOverlappedWithItem.Broadcast(TimeToAdd);
}
