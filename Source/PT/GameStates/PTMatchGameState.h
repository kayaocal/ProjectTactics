// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PTMatchGameState.generated.h"

/**
 * 
 */
UCLASS()
class PT_API APTMatchGameState : public AGameState
{
	GENERATED_BODY()

	virtual void AddPlayerState(APlayerState* PlayerState) override;
};
