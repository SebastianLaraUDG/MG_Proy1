// Proyecto 1 de motores graficos II. S, A y E.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MG_PROY1_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void HandleTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Health|Gameplay")
	float MaxHealth = 100.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Health|Gameplay")
	float CurrentHealth = 0.0f;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable,BlueprintReadOnly, Category="Health|Gameplay")
	FOnDeathSignature OnDeath;
	
	UFUNCTION(BlueprintCallable,blueprintPure,Category="Health|Gameplay")
	FORCEINLINE float GetHealthPercentage() const { return CurrentHealth / MaxHealth; }
};
