// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PT/Game/PTMatchPrerequisties.h"

#include "PTMatchGameState.generated.h"

/**
 * 
 */
UCLASS()
class PT_API APTMatchGameState : public AGameState
{
	GENERATED_BODY()

	APTMatchGameState ();
	
	virtual void AddPlayerState(APlayerState* PlayerState) override;

	virtual void Tick(float DeltaTime) override;

	public:

	UPROPERTY()
	TEnumAsByte<EMatchStatus> MatchStatus = EMatchStatus::Idle;


	void SetMatchStatus(EMatchStatus TargetStatus);
	EMatchStatus GetMatchStatus();
};
