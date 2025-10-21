// Proyecto 1 de motores graficos II. S, A y E.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Bind damage taken
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &ThisClass::HandleTakeDamage);
}

void UHealthComponent::HandleTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                        class AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHealth -= Damage;

	// Llama eventos vinculados
	if (CurrentHealth < 0.0f)
	{
		OnDeath.Broadcast();
	}
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
