// Fill out your copyright notice in the Description page of Project Settings.


#include "PTMatchGameState.h"
#include "PT/Prerequisties.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "PT/PTGameInstance.h"
#include "PT/GameModes/PTMatchGameMode.h"
#include "PT/PlayerControllers/PTMatchPlayerController.h"
#include "PT/PlayerStates/PTMatchPlayerState.h"
#include "PT/UI/TurnWidget.h"

APTMatchGameState::APTMatchGameState()
	:AGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APTMatchGameState::BeginPlay()
{
}

void APTMatchGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	if(HasAuthority())
	{
		LOG_W("Player state added for authory");	
	}
	else
	{
		LOG_W("Player state added for unauthory %s", *PlayerState->GetName());	
	}
}

void APTMatchGameState::Tick(float DeltaTime)
{
	if(!HasAuthority())
	{
		//LOG(" Current Match Status : %d", static_cast<int>(MatchStatus));
	}
}

void APTMatchGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APTMatchGameState, GameCondition);
	DOREPLIFETIME(APTMatchGameState, PlayerTurn);
}

void APTMatchGameState::SetGameCondition(EGameCondition TargetStatus)
{
	GameCondition = TargetStatus;
}

uint8 APTMatchGameState::GetGameCondition()
{
	return GameCondition;
}

void APTMatchGameState::OnRep_GameConditionChanged()
{
	if(!HasAuthority())
	{
		LOG(" UnAuthority Match status is changed to : %d ", GameCondition);
	}
	else
	{
		LOG(" Authority Match status is changed to : %d ", GameCondition);
	}
	
}

void APTMatchGameState::OnRep_PlayerTurnChanged()
{
	
	LOG(" UnAuthorized: PlayerTurn changed to : %d ", PlayerTurn);
	UPTGameInstance* GameInstance =  Cast<UPTGameInstance>(UGameplayStatics::GetGameInstance(this));

	if(GameInstance != nullptr && GameInstance->TurnUI != nullptr)
	{
		APTMatchPlayerController* MatchPlayerController =  Cast<APTMatchPlayerController>( UGameplayStatics::GetPlayerController(this,0));

		if(MatchPlayerController != nullptr)
		{
			APTMatchPlayerState* State = Cast<APTMatchPlayerState>(MatchPlayerController->PlayerState);

			LOG("turn change my team = %d", State->Team);

			if(State->Team == PlayerTurn +1 )
			{
				LOG("ENABLE BUTTON")
				GameInstance->TurnUI->EnableTurnButton();
			}
			else
			{
				LOG("DISABLE BUTTON")
				GameInstance->TurnUI->DisableTurnButton();
			}	
		}
	}
}

void APTMatchGameState::SetPlayerTurn(uint8 Turn)
{

	AGameModeBase* GMode = UGameplayStatics::GetGameMode(this);
	if(GMode == nullptr)
	{
		return;
	}
	
	PlayerTurn = Turn;
	PlayerTurn %= GMode->GetNumPlayers();
	if(HasAuthority())
	{
		OnRep_PlayerTurnChanged();
	}
}

uint8 APTMatchGameState::GetPlayerTurn()
{
	return PlayerTurn;
}

