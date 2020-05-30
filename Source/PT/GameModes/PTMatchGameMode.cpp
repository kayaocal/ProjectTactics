// Fill out your copyright notice in the Description page of Project Settings.


#include "PTMatchGameMode.h"
#include "PT/Prerequisties.h"
#include "PT/GameStates/PTMatchGameState.h"
#include "PT/PlayerControllers/PTMatchPlayerController.h"

APTMatchGameMode::APTMatchGameMode()
{
	GameStateClass = APTMatchGameState::StaticClass();
	PlayerControllerClass = APTMatchPlayerController::StaticClass();
}

void APTMatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	int NumOfPlayers = GetNumPlayers();
	Super::PostLogin(NewPlayer);

	if(HasAuthority())
	{
		LOG_W("Post Login is called for authory");
		APTMatchPlayerController* PTController = Cast<APTMatchPlayerController>(NewPlayer);
		if(PTController)
		{
			if(NumOfPlayers < ETeam::TeamCount && NumOfPlayers>=0)
			{
				PTController->Team = static_cast<ETeam>(NumOfPlayers);
				PTController->SetName(FString::Printf( TEXT("Player_%d"), NumOfPlayers));
			}
		}
	}
	else
	{
		LOG_W("Post Login is called for unauthory");
	}

}
