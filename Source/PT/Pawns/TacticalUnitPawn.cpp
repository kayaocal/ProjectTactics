// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticalUnitPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "PT/Prerequisties.h"

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
}

// Called when the game starts or when spawned
void ATacticalUnitPawn::BeginPlay()
{
	Super::BeginPlay();
	LOG("BEGIN PLAY");
}

// Called every frame
void ATacticalUnitPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATacticalUnitPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

