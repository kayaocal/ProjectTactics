// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PTMatchGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PT_API APTMatchGameMode : public AGameMode
{
	GENERATED_BODY()

	APTMatchGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;


};
