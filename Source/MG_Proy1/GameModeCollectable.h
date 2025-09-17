// Proyecto 1 de motores graficos II. S, A y E.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeCollectable.generated.h"

class ATargetHolder;
class AActor;
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

protected:
	virtual void BeginPlay() override;

	// Un objeto que almacena las posibles ubicaciones en el mundo.
	UPROPERTY(Transient, VisibleAnywhere)
	TObjectPtr<ATargetHolder> TargetHolder = nullptr;

	// The item we have to collect
	UPROPERTY(VisibleAnywhere, Category="Gameplay")
	TObjectPtr<AActor> Item;

	UFUNCTION()
	void OnItemBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetNumberOfItemsTaken() const { return NumberOfItemsTaken; }

private:
	void PlaceInRandomPositionInRange() const;
	int32 NumberOfItemsTaken = 0;
};
