// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticalUnitPawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PT/Prerequisties.h"
#include "PT/PlayerControllers/PTMatchPlayerController.h"

// Sets default values
ATacticalUnitPawn::ATacticalUnitPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	SetReplicates(true);
	SetReplicateMovement(true);
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(FName("Collider"));
	CapsuleComp->SetupAttachment(SkeletalMesh);
	CapsuleComp->SetRelativeLocation(FVector(0.0f,0.0f,90.0f));
	CapsuleComp->SetCapsuleSize(90.0f, false);
	CapsuleComp->SetCapsuleRadius(25.0f, false);

	MovementComp = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("MovementComponent"));
	MovementComp->SetIsReplicated(true);
	MovementComp->SetUpdatedComponent(CapsuleComp);
	AutoPossessAI = EAutoPossessAI::Spawned;
	
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
	LOG("BEGIN PLAY");
}

// Called every frame
void ATacticalUnitPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AAIController* AIController = Cast<AAIController>(GetController());
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

// Called to bind functionality to input
void ATacticalUnitPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATacticalUnitPawn::MovePawnToPos(FVector Pos)
{
	AAIController* AIController = Cast<AAIController>(GetController());

	if(AIController)
	{
		//AIController->SetReplicatingMovement(true);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "moving");
		AIController->MoveToLocation(FVector(Pos.X, Pos.Y, 0),0,false,true,false,true,0,false);
		if(AIController->IsReplicatingMovement())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "yesyes");
		}
		
	}
}

