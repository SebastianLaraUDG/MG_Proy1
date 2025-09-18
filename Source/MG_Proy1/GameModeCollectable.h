// Proyecto 1 de motores graficos II. S, A y E.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeCollectable.generated.h"

class ATargetHolder;
class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerOverlappedWithItemDelegate, float, TimeToAdd);
/**
 * Este game mode tiene dependencia directa a la clase TargetHolder.
 */
UCLASS(Abstract, NotPlaceable, Blueprintable, Transient)
class MG_PROY1_API AGameModeCollectable : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameModeCollectable();

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The item class. Make sure it generates overlap events."),
		Category = "Gameplay")
	TSubclassOf<AActor> ItemClass;

	// El tiempo restante antes de Game over. (Note: update only with events, not on Tick)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float TimeRemaining = 20.0f;

	// Time to add when player overlaps with items.
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Transient, Category = "Gameplay")
	float TimeToAdd = 10.0f;

protected:
	virtual void BeginPlay() override;

	// Un objeto que almacena las posibles ubicaciones en el mundo.
	UPROPERTY(Transient, VisibleAnywhere)
	TObjectPtr<ATargetHolder> TargetHolder = nullptr;

	// The item we have to collect
	UPROPERTY(VisibleAnywhere, Category="Gameplay")
	TObjectPtr<AActor> Item;
	
	UFUNCTION()
	void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetNumberOfItemsTaken() const { return NumberOfItemsTaken; }

	UPROPERTY(BlueprintAssignable)
	FOnPlayerOverlappedWithItemDelegate OnPlayerOverlappedWithItem;

private:
	void PlaceInRandomPositionInRange();
	int32 NumberOfItemsTaken = 0;
	int32 LastItemLocation = -1;
};
