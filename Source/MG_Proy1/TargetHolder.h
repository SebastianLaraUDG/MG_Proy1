// Proyecto 1 de motores graficos II. S, A y E.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetHolder.generated.h"

class ATargetPoint;

/*
 * An actor which holds a Target Point Actor list.
 */
UCLASS(Blueprintable,BlueprintType)
class MG_PROY1_API ATargetHolder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetHolder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Los puntos donde posiblemente apareceran los coleccionables.
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<TObjectPtr<ATargetPoint>> Points;
};
