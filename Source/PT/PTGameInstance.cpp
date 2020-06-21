// Fill out your copyright notice in the Description page of Project Settings.


#include "PTGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "PT/UI/TurnWidget.h"

UPTGameInstance::UPTGameInstance(const FObjectInitializer & ObjectInitializer)
{

    static ConstructorHelpers::FClassFinder<UUserWidget> TurnUIBPClass(TEXT("/Game/Widgets/TurnWidget"));

    if(TurnUIBPClass.Class != nullptr)
    {
        TurnUIClass = TurnUIBPClass.Class;
    }

    
}

void UPTGameInstance::LoadTurnUI()
{
    if(TurnUIClass == nullptr)
    {
        return;
    }

    TurnUI = CreateWidget<UTurnWidget> (this, TurnUIClass);

    if(TurnUI == nullptr)
    {
        return;
    }

    TurnUI->AddToViewport();
}
