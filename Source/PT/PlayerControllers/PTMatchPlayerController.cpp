// Fill out your copyright notice in the Description page of Project Settings.


#include "PTMatchPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PT/GameModes/PTMatchGameMode.h"
#include "PT/GameStates/PTMatchGameState.h"

void APTMatchPlayerController::BeginPlay()
{

}

void APTMatchPlayerController::Tick(float DeltaTime)
{
	
}

void APTMatchPlayerController::CE_MatchStatusIsChanged_Implementation(const uint8 NewMatchStatus)
{
	if(!HasAuthority())
	{
		LOG(" UnAuthority Match status is changed to : %d ", NewMatchStatus);
	}
	else
	{
		LOG(" Authority Match status is changed to : %d ", NewMatchStatus);
	}
	APTMatchGameState* GameState = Cast<APTMatchGameState>(UGameplayStatics::GetGameState(this));
	if(GameState)
	{
		GameState->SetMatchStatus(static_cast<EMatchStatus>(NewMatchStatus));
	}
	else
	{
		LOG_ERR(" Match status is changed but GameState is null");
	}
}
