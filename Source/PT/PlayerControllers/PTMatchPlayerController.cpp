// Fill out your copyright notice in the Description page of Project Settings.


#include "PTMatchPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PT/GameModes/PTMatchGameMode.h"
#include "PT/GameStates/PTMatchGameState.h"

APTMatchPlayerController::APTMatchPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
}


void APTMatchPlayerController::BeginPlay()
{

}

void APTMatchPlayerController::Tick(float DeltaTime)
{
	
}

