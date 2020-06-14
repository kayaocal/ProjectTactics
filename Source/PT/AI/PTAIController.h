// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Engine/TargetPoint.h"

#include "PTAIController.generated.h"

/**
 * 
 */
UCLASS()
class PT_API APTAIController : public AAIController
{
	GENERATED_BODY()

	APTAIController();

	   UPROPERTY()
        TArray<AActor*> Waypoints;

    UFUNCTION()
        ATargetPoint* GetRandomWaypoint();
protected:
	void BeginPlay() override;
	void GoToRandomWaypoint();

public:
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	
};
