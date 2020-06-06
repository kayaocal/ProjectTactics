// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PT/Prerequisties.h"
#include "GameFramework/PlayerController.h"
#include "PT/Game/PTGridFloor.h"

#include "PTMatchPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PT_API APTMatchPlayerController : public APlayerController
{
	GENERATED_BODY()

	public:

	APTMatchPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void UpdateSelectedGrid();
	void SetGridFloor(APTGridFloor* Grid);
	
	private:
	APTGridFloor* GridFloor = nullptr;
};
