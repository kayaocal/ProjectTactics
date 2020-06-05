// Fill out your copyright notice in the Description page of Project Settings.


#include "PTCameraSpectatorPawn.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Components/InputComponent.h"
#include "Engine/Classes/Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "PT/PlayerControllers/PTMatchPlayerController.h"

APTCameraSpectatorPawn::APTCameraSpectatorPawn(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	bAddDefaultMovementBindings = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bCanMoveCamera = true;

	// collision
	GetCollisionComponent()->SetGenerateOverlapEvents(false);
	GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCollisionComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCollisionComponent()->SetSimulatePhysics(false);

	CameraXYLimit = 7500.f;
	CameraHeight = 1000.f;
	CameraHeightMin = 5.f;
	CameraHeightMax = 5000.f;

	CameraRadius = 1100.f;
	CameraRadiusMin = 500.f;
	CameraRadiusMax = 8000.f;

	CameraZAngle = 0.f;
	CameraHeightAngle = 50.f;                // pitch
	CameraHeightAngleMin = 15.f;
	CameraHeightAngleMax = 85.f;


	CameraZoomSpeed = 200.f;
	CameraMovementSpeed = 3000.f;
	CameraRotationSpeed = 10.f;

	CameraScrollBoundary = 30.f; // screen edge margin

	CameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("RTS Camera"));
	CameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraComponent->bUsePawnControlRotation = false;
	RepositionCamera();
}

void APTCameraSpectatorPawn::LeftClickHandler()
{
	APTMatchPlayerController* PlayerController = Cast<APTMatchPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController == nullptr)
	{
		return;
	}

	// switch(PlayerController->GetControllerState())
	// {
	// 	case SELECTION:
	// 	SelectionPressed();
	// 	break;
	// 	
	// 	case CONSTRUCTION:
	// 	PlayerController->SetConstructionRotate(true);
	// 	break;
	//
	// 	default:
	// 	break;
	// }
	
}

void APTCameraSpectatorPawn::LeftClickReleaseHandler()
{
	APTMatchPlayerController* PlayerController = Cast<APTMatchPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController == nullptr)
	{
		return;
	}

	// switch(PlayerController->GetControllerState())
	// {
	// 	case SELECTION:
	// 	SelectionReleased();
	// 	break;
	// 	
	// 	case CONSTRUCTION:
	// 	PlayerController->BuildTemporaryUnit();
	// 	break;
	//
	// 	default:
	// 	break;
	// }
}

void APTCameraSpectatorPawn::RightClickHandler()
{
	APTMatchPlayerController* PlayerController = Cast<APTMatchPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController == nullptr)
	{
		return;
	}
}

void APTCameraSpectatorPawn::RightClickReleaseHandler()
{
	APTMatchPlayerController* PlayerController = Cast<APTMatchPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController == nullptr)
	{
		return;
	}
}

void APTCameraSpectatorPawn::SelectionPressed()
{
}

void APTCameraSpectatorPawn::SelectionReleased()
{
}

void APTCameraSpectatorPawn::RotateInput(float Direction)
{
	if (!bCanMoveCamera)
	{
		return;
	}

	RotateValue = FMath::Abs(Direction);
}

void APTCameraSpectatorPawn::ZoomInByWheel()
{
	if (!bCanMoveCamera) 
	{
		return;
	}

	CameraRadius -= CameraZoomSpeed;
	CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);
}

void APTCameraSpectatorPawn::ZoomOutByWheel()
{
	if (!bCanMoveCamera)
	{
		return;
	}

	CameraRadius += CameraZoomSpeed;
	CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);
}

void APTCameraSpectatorPawn::RotateLeftByWheel()
{
	if (!bCanMoveCamera)
	{
		return;
	}

	CameraZAngle -= CameraRotationSpeed;
}

void APTCameraSpectatorPawn::RotateRightByWheel()
{
	if (!bCanMoveCamera)
	{
		return;
	}

	CameraZAngle += CameraRotationSpeed;
}

void APTCameraSpectatorPawn::RotateUpByWheel()
{
	if (!bCanMoveCamera)
	{
		return;
	}

	CameraHeightAngle += CameraRotationSpeed;
	CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);
}

void APTCameraSpectatorPawn::RotateDownByWheel()
{
	if (!bCanMoveCamera)
	{
		return;
	}

    CameraHeightAngle -= CameraRotationSpeed;
    CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);
}

void APTCameraSpectatorPawn::MoveCameraForwardInput(float Direction)
{
	if (!bCanMoveCamera)
	{
		return;
	}

	MoveForwardValue = Direction;
}

void APTCameraSpectatorPawn::MoveCameraRightInput(float Direction)
{
	if (!bCanMoveCamera)
	{
		return;
	}

	MoveRightValue = Direction;
}

void APTCameraSpectatorPawn::MoveCameraUpInput(float Direction)
{
	if (!bCanMoveCamera)
	{
		return;
	}

	MoveUpValue = Direction;
}

void APTCameraSpectatorPawn::ZoomCameraInInput(float Direction)
{
	if (!bCanMoveCamera)
	{
		return;
	}

	ZoomInValue = Direction;
}

void APTCameraSpectatorPawn::RepositionCamera()
{
	FVector NewLocation(0.f, 0.f, 0.f);
	FRotator NewRotation(0.f, 0.f, 0.f);

	float sinCameraZAngle = FMath::Sin(FMath::DegreesToRadians(CameraZAngle));
	float cosCameraZAngle = FMath::Cos(FMath::DegreesToRadians(CameraZAngle));

	float sinCameraHeightAngle = FMath::Sin(FMath::DegreesToRadians(CameraHeightAngle));
	float cosCameraHeightAngle = FMath::Cos(FMath::DegreesToRadians(CameraHeightAngle));

	NewLocation.X = cosCameraZAngle * cosCameraHeightAngle * CameraRadius;
	NewLocation.Y = sinCameraZAngle * cosCameraHeightAngle * CameraRadius;
	NewLocation.Z = sinCameraHeightAngle * CameraRadius;
	
	float TerrainSurfaceZ = GetLandTerrainSurfaceAtCoord(GetActorLocation().X + NewLocation.X, GetActorLocation().Y + NewLocation.Y);
	if (GetActorLocation().Z + NewLocation.Z < TerrainSurfaceZ + CameraHeight)
	{
		NewLocation.Z = TerrainSurfaceZ - GetActorLocation().Z + CameraHeight;
	}

	CameraComponent->SetRelativeLocation(NewLocation);
	NewRotation = (FVector(0.0f, 0.0f, 0.0f) - NewLocation).Rotation();
	CameraComponent->SetRelativeRotation(NewRotation);
}

float APTCameraSpectatorPawn::GetLandTerrainSurfaceAtCoord(float XCoord, float YCoord) const
{
	FCollisionQueryParams TraceParams(FName(TEXT("LandTerrain")), false, this);        // TraceTag (info for debugging), bTraceComplex, AddIgnoredActor
	TraceParams.bFindInitialOverlaps = false;

	FHitResult Hit;

	FVector Start = FVector(XCoord, YCoord, GetActorLocation().Z + CameraRadius);
	FVector End = FVector(XCoord, YCoord, -500.f);

	// ECC_ channels should be set properly !!!
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_WorldStatic, TraceParams);

	if (bHit)
	{
		return Hit.ImpactPoint.Z;    // for shape trace it differs from Location
	}

	return 0.f;        // water level
}

void APTCameraSpectatorPawn::Tick(float DeltaSeconds)
{
		Super::Tick(DeltaSeconds);

	FVector2D MousePosition;
	FVector2D ViewportSize;
	
	if (!GEngine)
	{
		return;
	}

	UGameViewportClient* GameViewport = GEngine->GameViewport;

	// it is always nullptr on dedicated server
	if (!GameViewport)
	{
		return;
	}

	GameViewport->GetViewportSize(ViewportSize);

	// if viewport is focused, contains the mouse, and camera movement is allowed
	if (GameViewport->IsFocused(GameViewport->Viewport)
		&& GameViewport->GetMousePosition(MousePosition) && bCanMoveCamera)
	{
		if (MousePosition.X < CameraScrollBoundary)
		{
			MoveRightValue = -1.0f;
		}
		else if (ViewportSize.X - MousePosition.X < CameraScrollBoundary)
		{
			MoveRightValue = 1.0f;
		}

		if (MousePosition.Y < CameraScrollBoundary)
		{
			MoveForwardValue = 1.0f;
		}
		else if (ViewportSize.Y - MousePosition.Y < CameraScrollBoundary)
		{
			MoveForwardValue = -1.0f;
		}

		FVector ActualLocation = GetActorLocation();
		FVector ActualMovement = FVector::ZeroVector;
	
		if (RotateValue == 0.f)
		{
			ActualMovement += MoveCameraForward(MoveForwardValue * DeltaSeconds);
			ActualMovement += MoveCameraRight(MoveRightValue * DeltaSeconds);
		}
		ActualLocation += ActualMovement;

		CameraHeight += MoveCameraUp(MoveUpValue * DeltaSeconds);
		CameraHeight = FMath::Clamp(CameraHeight, CameraHeightMin, CameraHeightMax);

		float TerrainSurfaceZ = GetLandTerrainSurfaceAtCoord(ActualLocation.X, ActualLocation.Y);
		ActualLocation.Z = TerrainSurfaceZ + CameraHeight;

		ActualLocation.X = FMath::Clamp(ActualLocation.X, -CameraXYLimit, CameraXYLimit);
		ActualLocation.Y = FMath::Clamp(ActualLocation.Y, -CameraXYLimit, CameraXYLimit);

		SetActorLocation(ActualLocation);

		if (RotateValue != 0.f)
		{
			TurnCameraUp(MoveForwardValue * DeltaSeconds);
			TurnCameraRight(MoveRightValue * DeltaSeconds);
		}

		ZoomCameraIn(ZoomInValue * DeltaSeconds);
		RepositionCamera();
	}
}

FVector APTCameraSpectatorPawn::MoveCameraForward(float Direction)
{
	float MovementValue = Direction * CameraMovementSpeed;
	FVector DeltaMovement = MovementValue * GetIsolatedCameraYaw().Vector();
	return DeltaMovement;
}

FVector APTCameraSpectatorPawn::MoveCameraRight(float Direction)
{
	float MovementValue = Direction * CameraMovementSpeed;
	FVector DeltaMovement = MovementValue * (FRotator(0.0f, 90.0f, 0.0f) + GetIsolatedCameraYaw()).Vector();
	return DeltaMovement;
}


float APTCameraSpectatorPawn::MoveCameraUp(float Direction)
{
	float MovementValue = Direction * CameraMovementSpeed;
	return MovementValue;
}

void APTCameraSpectatorPawn::ZoomCameraIn(float Direction)
{
	float MovementValue = Direction * CameraMovementSpeed;
	CameraRadius += MovementValue;
	CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);
}

void APTCameraSpectatorPawn::TurnCameraUp(float Direction)
{
	CameraHeightAngle -= Direction * CameraRotationSpeed * 10.0f;
	CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);
}

void APTCameraSpectatorPawn::TurnCameraRight(float Direction)
{
	CameraZAngle += Direction * CameraRotationSpeed * 10.0f;
}

FRotator APTCameraSpectatorPawn::GetIsolatedCameraYaw()
{
	return FRotator(0.0f, CameraComponent->GetComponentToWorld().Rotator().Yaw, 0.0f);
}

void APTCameraSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (!PlayerInputComponent)
	{
		return;
	}

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ZoomOutByWheel", IE_Pressed, this, &APTCameraSpectatorPawn::ZoomOutByWheel);
	PlayerInputComponent->BindAction("ZoomInByWheel", IE_Pressed, this, &APTCameraSpectatorPawn::ZoomInByWheel);
	PlayerInputComponent->BindAction("RotateUpByWheel", IE_Pressed, this, &APTCameraSpectatorPawn::RotateUpByWheel);
	PlayerInputComponent->BindAction("RotateDownByWheel", IE_Pressed, this, &APTCameraSpectatorPawn::RotateDownByWheel);
	PlayerInputComponent->BindAction("RotateLeftByWheel", IE_Pressed, this, &APTCameraSpectatorPawn::RotateLeftByWheel);
	PlayerInputComponent->BindAction("RotateRightByWheel", IE_Pressed, this, &APTCameraSpectatorPawn::RotateRightByWheel);
	PlayerInputComponent->BindAxis("MoveForward", this, &APTCameraSpectatorPawn::MoveCameraForwardInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &APTCameraSpectatorPawn::MoveCameraRightInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &APTCameraSpectatorPawn::MoveCameraUpInput);
	PlayerInputComponent->BindAxis("ZoomIn", this, &APTCameraSpectatorPawn::ZoomCameraInInput);
	PlayerInputComponent->BindAxis("Rotate", this, &APTCameraSpectatorPawn::RotateInput);
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &APTCameraSpectatorPawn::LeftClickHandler);
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Released, this, &APTCameraSpectatorPawn::LeftClickReleaseHandler);
	PlayerInputComponent->BindAction("RightMouseClick", IE_Pressed, this, &APTCameraSpectatorPawn::RightClickHandler);
	PlayerInputComponent->BindAction("RightMouseClick", IE_Released, this, &APTCameraSpectatorPawn::RightClickReleaseHandler);

}
