// Proyecto 1 de motores graficos II. S, A y E.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeMonasChinas.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndGame);

UCLASS(Blueprintable, BlueprintType, Transient)
class MG_PROY1_API AGameModeMonasChinas : public AGameModeBase
{
	GENERATED_BODY()
	public :
		UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Gameplay")
		int32 puntuacionJugador = 0;
		UPROPERTY(BlueprintAssignable)
		FOnEndGame OnEndGame;
};
