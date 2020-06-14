// Fill out your copyright notice in the Description page of Project Settings.


#include "PTAIController.h"

#include "Kismet/GameplayStatics.h"
#include "PT/Prerequisties.h"

APTAIController::APTAIController()
{
	LOG("############# APTAICONTROLLER IS CREATED");
	
}

ATargetPoint* APTAIController::GetRandomWaypoint()
{
	auto index = FMath::RandRange(0, Waypoints.Num() - 1);
    return Cast<ATargetPoint>(Waypoints[index]);
}

void APTAIController::BeginPlay()
{
	Super::BeginPlay();

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), Waypoints);

	LOG("WP NUM : %d", Waypoints.Num());
	if(Waypoints.Num() > 0)
	{
	//	GoToRandomWaypoint();
	}
}

void APTAIController::GoToRandomWaypoint()
{
    MoveToLocation(GetRandomWaypoint()->GetActorLocation());
}



void APTAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if(RequestID.IsValid())
	{
		LOG("----------------------Valid");
		
	}
	else
	{
		LOG("----------------------Not Valid");
	}

	LOG("  Result.Flags : %d",  Result.Flags);
	if((FPathFollowingResultFlags::Success & Result.Flags) != 0)
	{
		LOG("----------------------Success");
	}
	if((FPathFollowingResultFlags::Blocked & Result.Flags) != 0)
	{
		LOG("----------------------Blocked");
	}
	if((FPathFollowingResultFlags::OffPath & Result.Flags) != 0)
	{
		LOG("----------------------OffPath");
	}
	if((FPathFollowingResultFlags::UserAbort & Result.Flags) != 0)
	{
		LOG("----------------------UserAbort");
	}
	if((FPathFollowingResultFlags::OwnerFinished & Result.Flags) != 0)
	{
		LOG("----------------------OwnerFinished");
	}
	if((FPathFollowingResultFlags::InvalidPath & Result.Flags) != 0)
	{
		LOG("----------------------InvalidPath");
	}
	if((FPathFollowingResultFlags::MovementStop & Result.Flags) != 0)
	{
		LOG("----------------------MovementStop");
	}
	if((FPathFollowingResultFlags::NewRequest & Result.Flags) != 0)
	{
		LOG("----------------------NewRequest");
	}
	if((FPathFollowingResultFlags::ForcedScript & Result.Flags) != 0)
	{
		LOG("----------------------ForcedScript");
	}
	if((FPathFollowingResultFlags::AlreadyAtGoal & Result.Flags) != 0)
	{
		LOG("----------------------AlreadyAtGoal");
	}
	
	LOG(" MOVE COMPLETED");

}
