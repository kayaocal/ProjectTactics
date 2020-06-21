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
protected:
	void BeginPlay() override;

private:

	virtual void AddPlayerState(APlayerState* PlayerState) override;

	virtual void Tick(float DeltaTime) override;

	public:

	UPROPERTY(ReplicatedUsing=OnRep_GameConditionChanged, BlueprintReadOnly)
	uint8 GameCondition = 255;

	
	UPROPERTY(ReplicatedUsing=OnRep_PlayerTurnChanged, BlueprintReadOnly)
	uint8 PlayerTurn = 255;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetGameCondition(EGameCondition TargetStatus);
	uint8 GetGameCondition();

	UFUNCTION()
	void OnRep_GameConditionChanged();
	UFUNCTION()
	void OnRep_PlayerTurnChanged();
	void SetPlayerTurn(uint8 Turn);
	uint8 GetPlayerTurn();
};
