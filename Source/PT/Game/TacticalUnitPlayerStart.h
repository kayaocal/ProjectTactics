// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "PT/Prerequisties.h"

#include "TacticalUnitPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class PT_API ATacticalUnitPlayerStart : public APlayerStart
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ETeam> Team;
	
};
