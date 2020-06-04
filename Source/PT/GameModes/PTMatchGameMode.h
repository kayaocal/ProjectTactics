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

	uint8 TargetGameCondition;
	uint8 TeamRound = TEAM_ROUND_NONE;
	
	void SetGameTargetCondition(EGameCondition TStatus);
	void SpawnUnits();
	void GameResumingConditionStarted();
	void ChangeGameCondition();

	void NextRound();
	TArray<AActor*> PlayerStartActors;

	int GetExceptedNumOfPlayers();


	
};
