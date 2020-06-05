// Fill out your copyright notice in the Description page of Project Settings.


#include "PTMatchGameMode.h"

#include "ClothingSystemEditor/Private/ClothingSystemEditorPrivatePCH.h"
#include "PT/Prerequisties.h"
#include "PT/GameStates/PTMatchGameState.h"
#include "PT/PlayerControllers/PTMatchPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PT/Camera/PTCameraSpectatorPawn.h"
#include "PT/Game/TacticalUnitPlayerStart.h"
#include "PT/Pawns/TacticalUnitPawn.h"
#include "PT/PlayerStates/PTMatchPlayerState.h"

APTMatchGameMode::APTMatchGameMode()
{
	GameStateClass = APTMatchGameState::StaticClass();
	PlayerControllerClass = APTMatchPlayerController::StaticClass();
	PlayerStateClass = APTMatchPlayerState::StaticClass();
	DefaultPawnClass = APTCameraSpectatorPawn::StaticClass();
	
	TeamRound = TEAM_ROUND_NONE;
	SetGameTargetCondition(EGameCondition::Idle);
	ChangeGameCondition();
	
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
					PState->Team = static_cast<uint8>(NumOfPlayers) + 1;
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
	
	if(TargetGameCondition != GameState->GameCondition)
	{
		HangOnStatusChangeTimer-=DeltaSeconds;
		if(HangOnStatusChangeTimer <= 0.0f)
		{
			ChangeGameCondition();
		}
		return;
	}
	
	if(GameState->GameCondition == EGameCondition::Idle)
	{
		const uint8 NumOfPlayers = static_cast<uint8>(GetNumPlayers());

		StatusTimer -= DeltaSeconds;
		if(NumOfPlayers == GetExceptedNumOfPlayers() || StatusTimer <= 0.0f)
		{
			//TODO: Check if match can started


			//Match is ready to start, so assign teams and stuff.

			
			HangOnStatusChangeTimer = HangOnIdleStatusChangeTime;
			SetGameTargetCondition(EGameCondition::PreResuming);
		}
	}
	else if(GameState->GameCondition == EGameCondition::PreResuming)
	{
		StatusTimer -= DeltaSeconds;
		if(StatusTimer <= 0.0f)
		{
			HangOnStatusChangeTimer = 1.0f;
			SetGameTargetCondition(EGameCondition::Resuming);
		}
	}

}

void APTMatchGameMode::SetGameTargetCondition(EGameCondition TCond)
{
	TargetGameCondition = static_cast<uint8>(TCond);
}

void APTMatchGameMode::SpawnUnits()
{
	LOG("SpanwUnits");
	const uint8 NumOfPlayers = static_cast<uint8>(GetNumPlayers());
	for(int i = 0; i < NumOfPlayers; i++)
	{
		uint8 TeamID = i + 1;
		LOG("TeamID : %d", TeamID);

		for( int j = 0; j < PlayerStartActors.Num(); j++)
		{
			LOG("j : %d", j);
			ATacticalUnitPlayerStart* UnitStartPoint = Cast<ATacticalUnitPlayerStart>(PlayerStartActors[j]);
			if(UnitStartPoint)
			{
			LOG("UnitStartPoint %d", UnitStartPoint->Team);
				if(UnitStartPoint->Team == TeamID)
				{
					LOG("TEAM IS SAME J");
					UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Blueprint/Characters/TacticalBP.TacticalBP")));

					UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
					if (!SpawnActor)
					{
					GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
					return;
					}

					UClass* SpawnClass = SpawnActor->StaticClass();
					if (SpawnClass == NULL)
					{
					GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
					return;
					}

					UWorld* World = GetWorld();
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, UnitStartPoint->GetActorLocation(), UnitStartPoint->GetActorRotation(), SpawnParams);

					
				//	ATacticalUnitPawn* Pawn = GetWorld()->SpawnActor<ATacticalUnitPawn>(ATacticalUnitPawn::StaticClass(), UnitStartPoint->GetActorLocation(), UnitStartPoint->GetActorRotation());
				//	Pawn->PossessedBy(ConnectedPlayerList[i]);
				}
			}

		}
	}
}

void APTMatchGameMode::GameResumingConditionStarted()
{
	StatusTimer = -1;
	LOG("GameResumingConditionStarted");
	/*Game just started. Spawn players*/
	if(TeamRound == TEAM_ROUND_NONE && HasAuthority())
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATacticalUnitPlayerStart::StaticClass(), PlayerStartActors);
		SpawnUnits();
	}
}

void APTMatchGameMode::ChangeGameCondition()
{
	LOG("Match Status is changed to %d ", TargetGameCondition);

	if(GameState)
	{
		GameState->SetGameCondition(static_cast<EGameCondition>(TargetGameCondition));
		switch(GameState->GameCondition)
		{
			case EGameCondition::Idle:
				StatusTimer = IdleStatusRemainingTime;
			break;
			case EGameCondition::PreResuming:
				StatusTimer = PreResumingStatusRemainingTime;
			break;
			case EGameCondition::Resuming:
				GameResumingConditionStarted();
				
			default:
				StatusTimer = -1;
			break;
	
		}
	
	}
}

void APTMatchGameMode::NextRound()
{
	if(TeamRound == TEAM_ROUND_NONE)
	{
		
	}
	
}

int APTMatchGameMode::GetExceptedNumOfPlayers()
{
	return 2;
}


