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

	// Time to add when player overlaps with items.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Category = "Gameplay")
	float TimeToAdd = 10.0f;

protected:
	virtual void BeginPlay() override;

	// Un objeto que almacena las posibles ubicaciones en el mundo.
	UPROPERTY(Transient, VisibleAnywhere)
	TObjectPtr<ATargetHolder> TargetHolder = nullptr;

	// The item we have to collect
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintReadOnly, Category="Gameplay")
	TObjectPtr<AActor> Item;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerOverlappedWithItemDelegate OnPlayerOverlappedWithItem;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 NumberOfItemsTaken = 0;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 NumberOfItemsForVictory = 5;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetNumberOfItemsTaken() const { return NumberOfItemsTaken; }

	UFUNCTION(BlueprintCallable)
	void PlaceInRandomPositionInRange(UPrimitiveComponent* Collider);

	UFUNCTION(BlueprintCallable)
	void CallPlayerOverlappedWithItemDelegate(UPrimitiveComponent* Collider);
private:
	int32 LastItemLocation = -1;
};
