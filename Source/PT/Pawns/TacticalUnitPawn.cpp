// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticalUnitPawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PT/Prerequisties.h"
#include "PT/AI/PTAIController.h"
#include "PT/PlayerControllers/PTMatchPlayerController.h"

// Sets default values
ATacticalUnitPawn::ATacticalUnitPawn()
	:Super()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = APTAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	SetReplicates(true);
	//SetReplicateMovement(true);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
//	GetCapsuleComponent()->SetRelativeLocation(FVector::ZeroVector);
	//GetMovementComponent()->SetIsReplicated(true);
	
}


// Called when the game starts or when spawned
void ATacticalUnitPawn::BeginPlay()
{
	Super::BeginPlay();
	APTMatchPlayerController* PlayerController = Cast<APTMatchPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if(PlayerController != nullptr)
	{
		PlayerController->SelectedCharacter = this;
	}
}

// Called every frame
void ATacticalUnitPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AAIController* AIController = Cast<AAIController>(GetController());
	if(AIController)
	{
		int aa =  AIController->GetMoveStatus();
		
		switch(aa)
		{
			case 0:
			GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow, "idle");
			break;
			case 1:
			GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow, "wait");
			break;
			case 2:
			GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow, "pause");
			break;
			case 3:
			GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow, "movemove");
			break;
		}
	}

}


uint8 ATacticalUnitPawn::GetTeam()
{
	return UnitConstantData.Team;
}

bool ATacticalUnitPawn::IsAlly(uint8 GivenTeam)
{
	return (GetTeam() == GivenTeam);
}

//-----------------------------------------------*--------*-----------------------------------------------
//---------------------------------------------- REPLICATION ----------------------------------------------
//-----------------------------------------------*---------*-----------------------------------------------

void ATacticalUnitPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATacticalUnitPawn, UnitConstantData);
}

void ATacticalUnitPawn::OnConstantDataChanged()
{
	LOG("########################################");
	LOG("Name : %s", *UnitConstantData.CharName);
	LOG("Team : %d", static_cast<int>(UnitConstantData.Team));
	LOG("CharID : %d", static_cast<int>(UnitConstantData.CharID));
	LOG("########################################");
	
}
