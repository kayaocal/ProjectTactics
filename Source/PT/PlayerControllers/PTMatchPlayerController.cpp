// Fill out your copyright notice in the Description page of Project Settings.


#include "PTMatchPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PT/GameModes/PTMatchGameMode.h"
#include "PT/GameStates/PTMatchGameState.h"

APTMatchPlayerController::APTMatchPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	PrimaryActorTick.bCanEverTick = true;
}


void APTMatchPlayerController::BeginPlay()
{

}

void APTMatchPlayerController::Tick(float DeltaTime)
{
	UpdateSelectedGrid();	
}

void APTMatchPlayerController::UpdateSelectedGrid()
{
	if(GridFloor == nullptr)
	{
		return;
	}
	
	PTGridManager* GridMan =  GridFloor->GetGridManager();
	if(GridMan == nullptr)
	{
		return;
	}
	
	FVector WorldLocation;
	FHitResult TraceResult(ForceInit);
	this->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, TraceResult);

	if(TraceResult.GetActor() == nullptr)
	{
		GridFloor->UpdateSelectedGrid(GridMan->GetGridBottomLeft(0), false);
		return;
	}

	int GridIndex = GridMan->WorldToGrid(TraceResult.ImpactPoint);

	int MaxIndex = GridFloor->ColumnCount*GridFloor->RowCount -1;
	if(GridIndex < 0 || GridIndex > MaxIndex)
	{
		GridFloor->UpdateSelectedGrid(GridMan->GetGridBottomLeft(0), false);
		return;
	}
	      GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, "ok");

	GridFloor->UpdateSelectedGrid(GridMan->GetGridBottomLeft(GridIndex), true);
}

void APTMatchPlayerController::SetGridFloor(APTGridFloor* Grid)
{
	GridFloor = Grid;
}

