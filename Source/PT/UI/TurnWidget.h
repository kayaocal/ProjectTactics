// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurnWidget.generated.h"

/**
 * 
 */
UCLASS()
class PT_API UTurnWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UButton* SkipTurnButton = nullptr;

	UFUNCTION(BlueprintCallable)
	void OnTurnButtonClicked();
	public:
	void DisableTurnButton();
	void EnableTurnButton();
};
