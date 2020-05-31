// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PT/Game/PTMatchPrerequisties.h"
#include "PTMatchGameMode.generated.h"

/**
 * 
 */


UCLASS()
class PT_API APTMatchGameMode : public AGameMode
{
	GENERATED_BODY()


	class APTMatchGameState* GameState;
	std::vector<class APTMatchPlayerController*> ConnectedPlayerList;
	
	float StatusTimer = 0;
	float HangOnStatusChangeTimer = 0;
	
	APTMatchGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void Tick(float DeltaSeconds) override;

	EMatchStatus TargetStatus;
	void SetMatchTargetStatus(EMatchStatus TStatus);
	void ChangeMatchStatus();


	int GetExceptedNumOfPlayers();


	
};
