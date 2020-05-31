// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PT/Prerequisties.h"
#include "GameFramework/PlayerController.h"
#include "PTMatchPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PT_API APTMatchPlayerController : public APlayerController
{
	GENERATED_BODY()

	public:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	

	
	/**
	 * RPC for notify all players that match status is changed...
	 *
	 * @param NewMatchStatus new status filled with EMatchStatus
	 */
	UFUNCTION(Client, Reliable)
	void CE_MatchStatusIsChanged(const uint8 NewMatchStatus);

	/**
	 * RPC implementation for getting New Status send from server
	 *
	 * @param NewMatchStatus new status filled with EMatchStatus
	 */
	UFUNCTION()
	void CE_MatchStatusIsChanged_Implementation(const uint8 NewMatchStatus);
	
};
