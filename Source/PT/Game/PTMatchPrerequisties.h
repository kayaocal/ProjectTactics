#pragma once

#include "UObject/ObjectMacros.h"

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
	One					= 0,	
	Two 				= 1,
	Three				= 2,
	Four				= 3,

	TeamCount
};

const int HangOnIdleStatusChangeTime = 3;
const int IdleStatusRemainingTime = 30;
const int PreResumingStatusRemainingTime = 5;




