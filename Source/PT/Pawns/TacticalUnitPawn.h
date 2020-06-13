// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "TacticalUnitPawn.generated.h"

UCLASS()
class PT_API ATacticalUnitPawn : public ACharacter
{
	GENERATED_BODY()

	
public:
	// Sets default values for this pawn's properties
	ATacticalUnitPawn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCharacterMovementComponent* MovementComp;
	
	uint8 Team;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MovePawnToPos(FVector Pos);
};
