// Proyectos de motores graficos II. S, A y E.


#include "FirstPersonCharacter.h"

#include "EnhancedInputComponent.h"
#include "GameModeMonasChinas.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "HealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DamageEvents.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->
		SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, 0.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	// Health component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->RegisterComponent();

	// configure movement
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

	// Weapon Mesh
	CurrentWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	CurrentWeaponMesh->SetupAttachment(FirstPersonCameraComponent);
}

void AFirstPersonCharacter::DoStartFiring()
{
	FVector Start, End;
	FHitResult HitResult;

	Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	End = Start + GetFirstPersonCameraComponent()->GetForwardVector() * MaxAimDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
#if WITH_EDITOR
		UKismetSystemLibrary::PrintString(this, TEXT("Impacto el linetrace!"));
		UKismetSystemLibrary::DrawDebugSphere(this,HitResult.ImpactPoint,10,12,FLinearColor::Yellow,8.0f);
#endif
		
		// Aplica destruccion e impulso a la fracture
		if (UGeometryCollectionComponent* GeometryCollectionComponent = HitResult.GetActor()->GetComponentByClass<
			UGeometryCollectionComponent>())
		{
			GeometryCollectionComponent->
				ApplyExternalStrain(HitResult.Item, HitResult.Location, 75.0f, 0, 0, 500000.0f);
			GeometryCollectionComponent->ApplyBreakingLinearVelocity(HitResult.Item,
			                                                         (End - Start).GetSafeNormal() * 2000.0f);
		}
		if (HitResult.GetActor())
		{
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(WeaponDamage, DamageEvent, GetController(), this);
			// Si hubiera dado tiempo de crear un sistema de armas, en lugar de 'this' seria el actor Weapon.
		}
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("NO Impacto el linetrace!"));
	}
}


void AFirstPersonCharacter::DoStopFiring()
{
	//
}

void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Bind so that when receiving damage we check if game mode should stop match.
	OnTakeAnyDamage.AddDynamic(this, &ThisClass::CheckIfIsDead);
}

// Called to bind functionality to input
void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);

		// Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ThisClass::DoStartFiring);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ThisClass::DoStopFiring);
	}
	else
	{
		UE_LOG(LogTemp, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void AFirstPersonCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
}


void AFirstPersonCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!WidgetClass) return;
	// Create Widget and add to viewport
	APlayerController* PlayerController = Cast<APlayerController>(NewController);
	Widget = CreateWidget(PlayerController, WidgetClass);
	Widget->AddToViewport(0);
}

void AFirstPersonCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AFirstPersonCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);
}

void AFirstPersonCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AFirstPersonCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AFirstPersonCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AFirstPersonCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void AFirstPersonCharacter::CheckIfIsDead(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                          class AController* InstigatedBy, AActor* DamageCauser)
{
	// Check if player died
	if (HealthComponent && HealthComponent->GetHealthPercentage() <= 0.0f)
	{
		if (AGameModeMonasChinas* const GameModeMonasChinas = Cast<AGameModeMonasChinas>(
			UGameplayStatics::GetGameMode(this)))
		{
			GameModeMonasChinas->OnEndGame.Broadcast();
		}
	}
}
