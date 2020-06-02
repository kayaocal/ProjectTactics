// Fill out your copyright notice in the Description page of Project Settings.


#include "PTMatchGameMode.h"

#include "ClothingSystemEditor/Private/ClothingSystemEditorPrivatePCH.h"
#include "PT/Prerequisties.h"
#include "PT/GameStates/PTMatchGameState.h"
#include "PT/PlayerControllers/PTMatchPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PT/PlayerStates/PTMatchPlayerState.h"

APTMatchGameMode::APTMatchGameMode()
{
	GameStateClass = APTMatchGameState::StaticClass();
	PlayerControllerClass = APTMatchPlayerController::StaticClass();
	PlayerStateClass = APTMatchPlayerState::StaticClass();
	
	SetMatchTargetStatus(EMatchStatus::Idle);
	ChangeMatchStatus();
	
	HasAuthority() ? PrimaryActorTick.bCanEverTick = true : PrimaryActorTick.bCanEverTick = false;
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
			ConnectedPlayerList.push_back(PTController);
			if(NumOfPlayers < ETeam::TeamCount && NumOfPlayers>=0)
			{
				APTMatchPlayerState* PState = Cast<APTMatchPlayerState>(PTController->PlayerState);
				if(PState)
				{
					PState->Team = static_cast<ETeam>(NumOfPlayers);
					PState->SetPlayerName(FString::Printf( TEXT("Player_%d"), NumOfPlayers));
				}
				else
				{
					LOG_ERR("Player State is null. Should not be");
				}
				
			}
		}
	}
	else
	{
		LOG_W("Post Login is called for unauthory");
	}

}

void APTMatchGameMode::Logout(AController* Exiting)
{
	if(HasAuthority())
	{
		LOG_W("Logout is called for authory");
	}
	else
	{
		LOG_W("Logout is called for unauthory");
	}

	for(int i = 0; i < ConnectedPlayerList.size(); i++)
	{
		if(ConnectedPlayerList[i] == Exiting)
		{
			LOG("\n Erasing Player from list...");
		}
	}
}

void APTMatchGameMode::Tick(float DeltaSeconds)
{
	if(GameState == nullptr)
	{
		GameState = Cast<APTMatchGameState>(UGameplayStatics::GetGameState(this));
		return;
	}
	
	if(TargetStatus != GameState->MatchStatus)
	{
		HangOnStatusChangeTimer-=DeltaSeconds;
		if(HangOnStatusChangeTimer <= 0.0f)
		{
			ChangeMatchStatus();
		}
		return;
	}
	
	if(GameState->MatchStatus == EMatchStatus::Idle)
	{
		const uint8 NumOfPlayers = static_cast<uint8>(GetNumPlayers());

		StatusTimer -= DeltaSeconds;
		if(NumOfPlayers == GetExceptedNumOfPlayers() || StatusTimer <= 0.0f)
		{
			//TODO: Check if match can started


			//Match is ready to start, so assign teams and stuff.

			
			HangOnStatusChangeTimer = HangOnIdleStatusChangeTime;
			SetMatchTargetStatus(EMatchStatus::PreResuming);
		}
	}
	else if(GameState->MatchStatus == EMatchStatus::PreResuming)
	{
		StatusTimer -= DeltaSeconds;
		if(StatusTimer <= 0.0f)
		{
			HangOnStatusChangeTimer = 1.0f;
			SetMatchTargetStatus(EMatchStatus::Resuming);
		}
	}

}

void APTMatchGameMode::SetMatchTargetStatus(EMatchStatus TStatus)
{
	TargetStatus = static_cast<uint8>(TStatus);
}

void APTMatchGameMode::ChangeMatchStatus()
{
	LOG("Match Status is changed to %d ", TargetStatus);

	if(GameState)
	{
		GameState->SetMatchStatus(static_cast<EMatchStatus>(TargetStatus));
		switch(GameState->MatchStatus)
		{
			case EMatchStatus::Idle:
				StatusTimer = IdleStatusRemainingTime;
			break;
			case EMatchStatus::PreResuming:
				StatusTimer = PreResumingStatusRemainingTime;
			break;
			default:
				StatusTimer = -1;
			break;
	
		}
	
	}
}

int APTMatchGameMode::GetExceptedNumOfPlayers()
{
	return 2;
}


