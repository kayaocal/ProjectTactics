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


public:
	void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
		FString& ErrorMessage) override;
private:
	class APTMatchGameState* GameState = nullptr;
	std::vector<class APTMatchPlayerController*> ConnectedPlayerList;
	
	FTimerManager* TimerManager = nullptr;
	FTimerHandle ActiveModeTimer;

private:
	APTMatchGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void Tick(float DeltaSeconds) override;

	uint8 TargetGameCondition;
	uint8 TeamRound = TEAM_ROUND_NONE;
	
	void SpawnUnits();
	void GameResumingConditionStarted();
	void ChangeGameCondition();
	void OnIdleTimerDone();
	void OnPreresumingTimerDone();
	void OnTimerDone();
	FTimerManager& GetTimeManager();
	void NextRound();
	TArray<AActor*> PlayerStartActors;

	int GetExceptedNumOfPlayers();

public:
	void SetGameTargetCondition(EGameCondition TCond, float TimeToWait);
	
};
