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

	UPROPERTY(ReplicatedUsing=OnRep_GameConditionChanged, BlueprintReadOnly)
	uint8 GameCondition = static_cast<uint8>(EGameCondition::Idle);


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetGameCondition(EGameCondition TargetStatus);
	uint8 GetGameCondition();

	UFUNCTION()
	void OnRep_GameConditionChanged();
};
