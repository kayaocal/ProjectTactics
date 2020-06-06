// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PT/Game/PTGrid.h"

/**
 * 
 */
class PT_API PTGridManager
{
public:
	PTGridManager(FVector2D StartPos,float GridSize,int ColumnCount, int RowCount);
	~PTGridManager();
	int WorldToGrid(FVector WorldPos);
	FVector GetGridCenter(int Index);
	FVector GetGridTopMid(int Index);
	FVector GetGridLeftMid(int Index);
	FVector GetGridTopLeft(int Index);
	FVector GetGridBottomLeft(int Index);
	void SetGridState(int Index, EGridState NewState);
	void SetGridStates(TArray<PTGrid*> GridsToSet, EGridState NewState);
	TArray<PTGrid*> GetGridsFromCenter(int Index, int RowCount, int ColumnCount);
	TArray<PTGrid*> GetNeighbours(int Index, int RowCount, int ColumnCount);
	FVector GetPositionToPlace(int Index, int RowCount, int ColumnCount);
	bool IsPlaceable(TArray<PTGrid*> GridsToPlace, EGridState RequiredState);

	TArray<PTGrid> PTGrids;
private:
	FVector2D StartPos;
	int ColumnCount;
	int RowCount;
	int GridSize;
};
