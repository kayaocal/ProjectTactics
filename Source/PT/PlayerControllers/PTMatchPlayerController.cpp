// Fill out your copyright notice in the Description page of Project Settings.


#include "PTMatchPlayerController.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "PT/Prerequisties.h"
#include "PT/PlayerStates/PTMatchPlayerState.h"

APTMatchPlayerController::APTMatchPlayerController()
	:Super()
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

	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, "ok");
	SelectedGridIndex = GridIndex;
	GridFloor->UpdateSelectedGrid(GridMan->GetGridBottomLeft(GridIndex), true);
}

void APTMatchPlayerController::SetGridFloor(APTGridFloor* Grid)
{
	GridFloor = Grid;
}

void APTMatchPlayerController::SelectCharAtMousePos()
{
	FHitResult TraceResult(ForceInit);
	this->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, TraceResult);
	
	if (TraceResult.GetActor() != nullptr)
	{	
		ATacticalUnitPawn* HitCharacter = Cast<ATacticalUnitPawn>(TraceResult.GetActor());			
		if(HitCharacter != nullptr)
		{
			APTMatchPlayerState* State = Cast<APTMatchPlayerState>(PlayerState);
			if(State)
			{
				if(HitCharacter->IsAlly(State->Team))
				{
					SelectedCharacter = HitCharacter;
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "SELECTED");
					
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "ENEMY CANT SELECTED");
				}
			}
			
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "not char");
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TraceResult.GetActor()->GetName());
		}
	}
}

void APTMatchPlayerController::MoveSelectedChar()
{
	if(SelectedCharacter == nullptr && SelectedGridIndex < 0 && GridFloor == nullptr)
	{
		return;
	}

	PTGridManager* GridMan =  GridFloor->GetGridManager();
	if(GridMan == nullptr)
	{
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Move");
	LOG("Selected Index %d", SelectedGridIndex);

	UnitMoveCommand.CharID = SelectedCharacter->UnitConstantData.CharID;
	UnitMoveCommand.Location = GridMan->GetGridCenter(SelectedGridIndex);
	RPC_MoveToPos(UnitMoveCommand);

}

void APTMatchPlayerController::RPC_MoveToPos_Implementation(FUnitMoveCommand Vec)
{
	TArray<AActor*> Arr;
	//TODO: Unnecessary performance impact. Make a better way to search units on scene
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATacticalUnitPawn::StaticClass(), Arr);
	LOG_W("RPC_MoveToPos_Implementation %d ", Arr.Num());

	if(HasAuthority())
	{
		LOG("Authory Imp");
	}
	for(int i = 0; i < Arr.Num(); i++)
	{
		if(Arr[i])
		{
			ATacticalUnitPawn* Unit = Cast<ATacticalUnitPawn>(Arr[i]);
			if(Unit)
			{
				if(Unit->UnitConstantData.CharID == Vec.CharID)
				{
					LOG("MOVE BITCH");
					AAIController* AIController = Cast<AAIController>(Unit->GetController());
					if(AIController)
					{
						AIController->MoveToLocation(Vec.Location);
					}
				}
			}
		}
	}
}


