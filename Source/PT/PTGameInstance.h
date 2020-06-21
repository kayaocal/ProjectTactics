// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PTGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PT_API UPTGameInstance : public UGameInstance
{
public:
	UPTGameInstance(const FObjectInitializer & ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void LoadTurnUI();
	
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UTurnWidget* TurnUI;
private:
	GENERATED_BODY()

	TSubclassOf<class UUserWidget> TurnUIClass;

};
