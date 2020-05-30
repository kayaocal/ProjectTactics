// Fill out your copyright notice in the Description page of Project Settings.


#include "PTMatchGameMode.h"
#include "PT/Prerequisties.h"
#include "PT/GameStates/PTMatchGameState.h"

APTMatchGameMode::APTMatchGameMode()
{
	GameStateClass = APTMatchGameState::StaticClass();
}

void APTMatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(HasAuthority())
	{
		LOG_W("Post Login is called for authory");
	}
	else
	{
		LOG_W("Post Login is called for unauthory");
	}
}
