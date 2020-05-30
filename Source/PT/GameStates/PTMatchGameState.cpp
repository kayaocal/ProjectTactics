// Fill out your copyright notice in the Description page of Project Settings.


#include "PTMatchGameState.h"
#include "PT/Prerequisties.h"


void APTMatchGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	
	if(HasAuthority())
	{
		LOG_W("Player state added for authory");	
	}
	else
	{
		LOG_W("Player state added for unauthory");	
	}
}
