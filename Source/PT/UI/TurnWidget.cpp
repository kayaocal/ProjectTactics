// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnWidget.h"

#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/Button.h"
#include "PT/Prerequisties.h"
#include "PT/GameStates/PTMatchGameState.h"
#include "PT/PlayerControllers/PTMatchPlayerController.h"


void UTurnWidget::OnTurnButtonClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, "clicked");
    APTMatchPlayerController* PlayerController = Cast<APTMatchPlayerController>(UGameplayStatics::GetPlayerController(this,0));

    if(PlayerController != nullptr)
    {
        PlayerController->RPC_SkipTurn();
    }
}

void UTurnWidget::DisableTurnButton()
{
    if(SkipTurnButton == nullptr)
    {
        return;
    }

    SkipTurnButton->SetIsEnabled(false);
}

void UTurnWidget::EnableTurnButton()
{
    if(SkipTurnButton == nullptr)
    {
        return;
    }
    SkipTurnButton->SetIsEnabled(true);
}
