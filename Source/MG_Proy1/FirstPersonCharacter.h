// Proyectos de motores graficos II. S, A y E.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FirstPersonCharacter.generated.h"


class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UHealthComponent;


/*
 * The base of a first person character.
 */
UCLASS(BlueprintType, Blueprintable)
class MG_PROY1_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Health Component */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components|Health", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> HealthComponent;

	
protected:
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> MouseLookAction;

	/** Fire weapon input action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* FireAction;

	/** Max distance to use for aim traces */
	UPROPERTY(EditAnywhere, Category ="Aim", meta = (ClampMin = 0, ClampMax = 100000, Units = "cm"))
	float MaxAimDistance = 10000.0f;

	/** Weapon currently equipped and ready to shoot with */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<UStaticMeshComponent> CurrentWeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="UI")
	TObjectPtr<UUserWidget> Widget;


public:
	AFirstPersonCharacter();

	UFUNCTION(BlueprintCallable, Category="Input")
	void DoStartFiring();

	UFUNCTION(BlueprintCallable, Category="Input")
	void DoStopFiring();

protected:

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	UFUNCTION()
	virtual void CheckIfIsDead(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void NotifyControllerChanged() override;

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Gameplay|Weapons")
	float WeaponDamage = 5.0f;
	
	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};
