#pragma once

#include "UObject/ObjectMacros.h"

/* Round system is not started yet. Time to spawn player's units */
#define TEAM_ROUND_NONE 255

/* Time limit for round system to change */
#define TEAM_ROUND_NONE_SECS 5

/* All players can play at this round freely */
#define TEAM_ROUND_ALL  254

#define TEAM_ROUND_ALL_SECS 60




UENUM(BlueprintType)
enum EGameCondition
{
	Idle = 0,
	PreResuming = 1,
	Resuming = 2,
	Paused = 3,
	Resulting = 4,

	ConditionCount
};

UENUM(BlueprintType)
enum ETeam
{
	None				= 0,
	One					= 1,	
	Two 				= 2,
	Three				= 3,
	Four				= 4,

	TeamCount
};

const int HangOnIdleStatusChangeTime = 3;
const int IdleStatusRemainingTime = 30;
const int PreResumingStatusRemainingTime = 5;

const int MaxTeamCount = 2;




