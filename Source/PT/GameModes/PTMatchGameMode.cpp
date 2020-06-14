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

void APTMatchGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}



APTMatchGameMode::APTMatchGameMode()
	:Super()
{
	GameStateClass = APTMatchGameState::StaticClass();
	PlayerControllerClass = APTMatchPlayerController::StaticClass();
	PlayerStateClass = APTMatchPlayerState::StaticClass();
	DefaultPawnClass = APTCameraSpectatorPawn::StaticClass();
	
	TeamRound = TEAM_ROUND_NONE;
	TargetGameCondition = 255;

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

				//All players are connected, So game can start!
				if(NumOfPlayers+1 == GetExceptedNumOfPlayers())
				{
					OnIdleTimerDone();
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
		LOG("Game state is null");
		AGameStateBase* GS = UGameplayStatics::GetGameState(this);
		if(GS)
		{
			GameState = Cast<APTMatchGameState>(GS);
			SetGameTargetCondition(EGameCondition::Idle, 1.0f);
		}
		return;
	}

}

void APTMatchGameMode::SetGameTargetCondition(EGameCondition TCond, float TimeToWait)
{
	TargetGameCondition = static_cast<uint8>(TCond);

	LOG(" SetGameTargetCondition");
	if(TargetGameCondition != GameState->GameCondition)
	{
		
		GetTimeManager().SetTimer(ActiveModeTimer, this, &APTMatchGameMode::ChangeGameCondition, TimeToWait, false);
	}
}

void APTMatchGameMode::SpawnUnits()
{
	LOG("SpanwUnits");
	const uint8 NumOfPlayers = static_cast<uint8>(GetNumPlayers());
	static uint8 NumOfSpawnedUnits = 0;
	
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
					SpawnParams.Owner = ConnectedPlayerList[i];
					
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					AActor* ac = World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, UnitStartPoint->GetActorLocation(), UnitStartPoint->GetActorRotation(), SpawnParams);
				
					ATacticalUnitPawn* Pawn = Cast<ATacticalUnitPawn>(ac);
					if(Pawn)
					{
						Pawn->UnitConstantData = FUnitConstantData{TeamID,NumOfSpawnedUnits, "Sir Ian McAllen"};
						NumOfSpawnedUnits++;
					}
					
				//	ATacticalUnitPawn* Pawn = GetWorld()->SpawnActor<ATacticalUnitPawn>(ATacticalUnitPawn::StaticClass(), UnitStartPoint->GetActorLocation(), UnitStartPoint->GetActorRotation());
				}
			}

		}
	}
}

void APTMatchGameMode::GameResumingConditionStarted()
{
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
				LOG(" Condition Idle Status Timer : %f", IdleStatusRemainingTime);
				GetTimeManager().SetTimer(ActiveModeTimer, this, &APTMatchGameMode::OnIdleTimerDone, IdleStatusRemainingTime, false);
			break;
			case EGameCondition::PreResuming:
				LOG(" Condition PreResuming Status Timer : %f", PreResumingStatusRemainingTime);
				GetTimeManager().SetTimer(ActiveModeTimer, this, &APTMatchGameMode::OnPreresumingTimerDone, PreResumingStatusRemainingTime, false);
			break;
			case EGameCondition::Resuming:
				GameResumingConditionStarted();

			break;
	
		}

		
	}
}

void APTMatchGameMode::OnIdleTimerDone()
{
	LOG("Idle Condition Timer is DONE!!");
	SetGameTargetCondition(EGameCondition::PreResuming, HangOnIdleStatusChangeTime);

}

void APTMatchGameMode::OnPreresumingTimerDone()
{
	LOG("Preresuming Condition Timer is DONE!!");
	SetGameTargetCondition(EGameCondition::Resuming, 1.0f);
}

void APTMatchGameMode::OnTimerDone()
{
	LOG("On Timer Done");
}

FTimerManager& APTMatchGameMode::GetTimeManager()
{
	return GetWorld()->GetTimerManager();
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

