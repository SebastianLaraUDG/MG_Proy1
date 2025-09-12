// Proyecto 1 de motores graficos II. S, A y E.


#include "GameModeCollectable.h"
#include "Kismet/GameplayStatics.h"
#include "TargetHolder.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Character.h"

AGameModeCollectable::AGameModeCollectable()
{
}

void AGameModeCollectable::BeginPlay()
{
	Super::BeginPlay();

	if (!ItemClass)
		return;
	
	TArray<AActor*> OutActors;

	// Find the Target Holder
	UGameplayStatics::GetAllActorsOfClass(
		this,
		ATargetHolder::StaticClass(),
		OutActors
	);

	// Save a reference to them (if found)
	if (!OutActors.IsEmpty())
	{
		// En teoria deberia haber solo una instancia de target holer en el nivel.
		// Por eso esta codificado especificamente para usar [0]
		if (ATargetHolder* TH = Cast<ATargetHolder>(OutActors[0]))
		{
			TargetHolder = TH;
		}
		check(TargetHolder!=nullptr);
	}

	// Spawn item
	if (UWorld* const World = GetWorld())
	{
		Item = World->SpawnActor<AActor>(ItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}

	// Place in a random location
	PlaceInRandomPositionInRange();

	// Bind overlap
	if (Item)
	{
		Item->OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnItemBeginOverlap);
	}
}

void AGameModeCollectable::OnItemBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Collided with player
	if (OverlappedActor == UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		// Change position of item
		PlaceInRandomPositionInRange();
		++NumberOfItemsTaken;
		UE_LOG(LogTemp, Display, TEXT("Number of items taken = %d"), NumberOfItemsTaken);
		// TODO: other stuff of the project I don't remember.
	}
}

void AGameModeCollectable::PlaceInRandomPositionInRange()
{
	if (!TargetHolder || TargetHolder->Points.IsEmpty())
	{
		UE_LOG(LogTemp, Display, TEXT("No target points found"));
		return;
	}
	// Place in a random location
	int32 PossibleLocation = FMath::RandRange(0, TargetHolder->Points.Num() - 1);
	Item->SetActorLocation(TargetHolder->Points[PossibleLocation]->GetActorLocation());
}
