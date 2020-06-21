// Fill out your copyright notice in the Description page of Project Settings.


#include "PTGridFloor.h"


#include "Kismet/GameplayStatics.h"
#include "PT/Prerequisties.h"
#include "PT/PlayerControllers/PTMatchPlayerController.h"

// Sets default values
APTGridFloor::APTGridFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootCompoenent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(RootCompoenent);
	SelectionMesh = CreateDefaultSubobject<UProceduralMeshComponent>("SelectionMesh");
	SelectionMesh->SetupAttachment(RootComponent);
	GridMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	GridMesh->SetupAttachment(RootComponent);
}

PTGridManager* APTGridFloor::GetGridManager() const
{
	return GridManager;
}

void APTGridFloor::SetGridManager(PTGridManager* NewGridManager)
{
	this->GridManager = NewGridManager;
}

// Called when the game starts or when spawned
void APTGridFloor::BeginPlay()
{
	Super::BeginPlay();
	APTMatchPlayerController* PlayerController = Cast<APTMatchPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "setset");
		PlayerController->SetGridFloor(this);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "setfail");
	}
	FVector2D StartPos = FVector2D(this->GetActorLocation().X,this->GetActorLocation().Y);
	StartPos.X += this->RowCount * this->GridSize;
	GridManager = new PTGridManager(StartPos,this->GridSize,this->ColumnCount, this->RowCount);
}

// Called every frame
void APTGridFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UMaterialInstanceDynamic* APTGridFloor::CreateMaterialInstance(FLinearColor Color, float Opacity)
{
	if(GridMesh==nullptr)
	{
		return nullptr;
	}

	UMaterialInstanceDynamic* GridMat = GridMesh->CreateAndSetMaterialInstanceDynamic(0);
	if(GridMat != nullptr)
	{
		GridMat->SetVectorParameterValue(FName("Color"), Color);
		GridMat->SetScalarParameterValue(FName("Opacity"), Opacity);
	}

	return GridMat;

	
	
}

void APTGridFloor::CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector>& Vertices,
	TArray<int>& Triangles)
{
	float HalfThickness = Thickness/2;

	FVector ThicknessDirection = FVector(End - Start);
	ThicknessDirection.Normalize(0.0001);
	ThicknessDirection = ThicknessDirection.CrossProduct(ThicknessDirection,FVector(0,0,1));

	Triangles.Add(Vertices.Num() + 2 );
	Triangles.Add(Vertices.Num() + 1 );
	Triangles.Add(Vertices.Num() + 0 );
	Triangles.Add(Vertices.Num() + 2 );
	Triangles.Add(Vertices.Num() + 3 );
	Triangles.Add(Vertices.Num() + 1 );

	Vertices.Add(Start + ThicknessDirection*HalfThickness);
	Vertices.Add(End + ThicknessDirection*HalfThickness);
	Vertices.Add(Start - ThicknessDirection*HalfThickness);
	Vertices.Add(End - ThicknessDirection*HalfThickness);
}

void APTGridFloor::OnConstruction(const FTransform& Transform)
{
	UMaterialInstanceDynamic* LineMaterialInstance = CreateMaterialInstance(LineColor, LineOpacity);
	UMaterialInstanceDynamic* GridMaterialInstance =SelectionMesh->CreateAndSetMaterialInstanceDynamic(0);

	if(GridMaterialInstance != nullptr)
	{
		GridMaterialInstance->SetVectorParameterValue(FName("Color"), GridColor);
		GridMaterialInstance->SetScalarParameterValue(FName("Opacity"), GridOpacity);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "NULL MAT");
	}

	int i = 0;
	TArray<FVector> LineVertices;
	TArray<int> LineTriangles;
	for(i = 0; i <= RowCount; i++)
	{
		float LineStart = i*GridSize;
		float LineEnd = ColumnCount*GridSize;
		CreateLine(FVector(LineStart,0,0),FVector(LineStart,LineEnd,0),LineSize,LineVertices, LineTriangles);
	}

	for(i = 0; i <= ColumnCount; i++)
	{
		float LineStart = i*GridSize;
		float LineEnd = RowCount*GridSize;
		CreateLine(FVector(0,LineStart,0),FVector(LineEnd,LineStart,0),LineSize,LineVertices, LineTriangles);
	}

	GridMesh->CreateMeshSection_LinearColor(0,LineVertices,LineTriangles,TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(),false);

	TArray<FVector> SelectionVertices;
	TArray<int> SelectionTriangles;
	CreateLine(FVector(0,GridSize/2,0), FVector(GridSize,GridSize/2,0),GridSize,SelectionVertices, SelectionTriangles);
	SelectionMesh->CreateMeshSection_LinearColor(0,SelectionVertices,SelectionTriangles,TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(),false);
	SelectionMesh->SetVisibility(false);

	/*APTMatchPlayerController* PlayerController = Cast<APTMatchPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "setset");
		PlayerController->SetGridFloor(this);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "setfail");
	}*/
}

void APTGridFloor::UpdateSelectedGrid(FVector NewPos, bool IsVisible)
{
	if(SelectionMesh == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "null");
		return;
	}

	SelectionMesh->SetWorldLocation(FVector(NewPos.X, NewPos.Y, NewPos.Z + 25));
	SelectionMesh->SetVisibility(IsVisible);
	
}

