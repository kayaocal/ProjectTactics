// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "TacticalUnitPawn.generated.h"



USTRUCT(BlueprintType)
struct FUnitConstantData
{
	GENERATED_BODY()

	//TODO: This struct could be optimized for network traffic!
	UPROPERTY(BlueprintReadOnly)
	uint8 Team;
	UPROPERTY(BlueprintReadOnly)
	uint8 CharID;
	UPROPERTY(BlueprintReadOnly)
	FString CharName;
	
};


UCLASS()
class PT_API ATacticalUnitPawn : public ACharacter
{
	GENERATED_BODY()

	
public:
	// Sets default values for this pawn's properties
	ATacticalUnitPawn();

	/* This is responsible of holding constant datas for pawns*/
	UPROPERTY(ReplicatedUsing=OnConstantDataChanged, BlueprintReadOnly)
	FUnitConstantData UnitConstantData;

	UFUNCTION()
	void OnConstantDataChanged();
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	uint8 GetTeam();

	bool IsAlly(uint8 GivenTeam);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
