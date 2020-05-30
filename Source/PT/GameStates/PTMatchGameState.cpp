// Fill out your copyright notice in the Description page of Project Settings.


#include "PTMatchGameState.h"
#include "PT/Prerequisties.h"
#include "GameFramework/PlayerState.h"


APTMatchGameState::APTMatchGameState()
	:AGameState()
{
//	PrimaryActorTick.bCanEverTick = true;
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

}
