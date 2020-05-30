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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ETeam> Team = ETeam::TeamCount;

	ETeam OldTeam = ETeam::TeamCount;
	
};
