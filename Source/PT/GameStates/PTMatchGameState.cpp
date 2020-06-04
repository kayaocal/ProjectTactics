// Fill out your copyright notice in the Description page of Project Settings.


#include "PTMatchGameState.h"
#include "PT/Prerequisties.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

APTMatchGameState::APTMatchGameState()
	:AGameState()
{
	PrimaryActorTick.bCanEverTick = true;
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
