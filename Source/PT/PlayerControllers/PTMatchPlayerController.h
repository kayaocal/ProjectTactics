// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PT/Prerequisties.h"
#include "GameFramework/PlayerController.h"
#include "PT/Game/PTGridFloor.h"
#include "PT/Pawns/TacticalUnitPawn.h"


#include "PTMatchPlayerController.generated.h"



USTRUCT(BlueprintType)
struct FUnitMoveCommand
{
	GENERATED_BODY()

	//TODO: This struct could be optimized for network traffic!
	UPROPERTY(BlueprintReadOnly)
	uint8 CharID;
	UPROPERTY(BlueprintReadOnly)
	FVector Location;
	
};

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
	void SelectCharAtMousePos();
	void MoveSelectedChar();

	bool IsItMyTurn();
	
	ATacticalUnitPawn* SelectedCharacter = nullptr;

	FUnitMoveCommand UnitMoveCommand;
	
	UFUNCTION(Server, Reliable)
	void RPC_MoveToPos(FUnitMoveCommand MoveCommand);

	UFUNCTION(Server, Reliable)
	void RPC_SkipTurn();

private:
	APTGridFloor* GridFloor = nullptr;
	int SelectedGridIndex = -1;
};
