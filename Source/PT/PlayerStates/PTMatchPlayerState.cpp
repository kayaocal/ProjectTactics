// Fill out your copyright notice in the Description page of Project Settings.


#include "PTMatchPlayerState.h"
#include "PT/Prerequisties.h"
#include "Net/UnrealNetwork.h"

APTMatchPlayerState::APTMatchPlayerState()
{
	SetReplicates(true);
	Team = 0;
}

void APTMatchPlayerState::OnRep_TeamIsChanged()
{
	LOG("ID %d, Player %s's Team is changed to %d", GetPlayerId(), *GetPlayerName(), static_cast<int>(Team));
	
}

void APTMatchPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APTMatchPlayerState, Team);
}
