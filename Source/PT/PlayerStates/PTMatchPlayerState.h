// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PT/Game/PTMatchPrerequisties.h"
#include "PTMatchPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PT_API APTMatchPlayerState : public APlayerState
{
	GENERATED_BODY()
public:

	APTMatchPlayerState();
	
	UPROPERTY(ReplicatedUsing=OnRep_TeamIsChanged, BlueprintReadOnly)
	uint8 Team = 0;

	UFUNCTION()
	void OnRep_TeamIsChanged();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
