// Fill out your copyright notice in the Description page of Project Settings.


#include "PTGridManager.h"
#include "PT/Prerequisties.h"


PTGridManager::PTGridManager(FVector2D StartPos, float GridSize, int ColumnCount, int RowCount)
{
    PTGrids.Reserve(ColumnCount*RowCount);
    for(int i = 0; i < ColumnCount*RowCount; i++)
    {
        PTGrids.Emplace(i,EGridState::Empty);
    }
    this->StartPos = StartPos;
    this->GridSize = GridSize;
    this->ColumnCount = ColumnCount;
    this->RowCount = RowCount;
}

PTGridManager::~PTGridManager()
{
}

int PTGridManager::WorldToGrid(FVector WorldPos)
{
    int row = (StartPos.X - WorldPos.X)/GridSize;
    int col = (WorldPos.Y - StartPos.Y)/GridSize;
    if(row >= RowCount || col >= ColumnCount || row < 0 || col < 0 || WorldPos.Y < StartPos.Y || StartPos.X < WorldPos.X)
    {
        return -1;
    }
    
    return row*ColumnCount + col;
}

FVector PTGridManager::GetGridCenter(int Index)
{
    int row = Index/ColumnCount;
    int col = Index%ColumnCount;

    return FVector(StartPos.X - row*GridSize - GridSize/2, StartPos.Y + col*GridSize + GridSize/2,0);
}

FVector PTGridManager::GetGridTopMid(int Index)
{
    return  GetGridCenter(Index) + FVector(GridSize/2,0,0);
}

FVector PTGridManager::GetGridLeftMid(int Index)
{
    return  GetGridCenter(Index) + FVector(0,-GridSize/2,0);
}

FVector PTGridManager::GetGridTopLeft(int Index)
{
    return GetGridCenter(Index) + FVector(GridSize/2,-GridSize/2,0);
}

FVector PTGridManager::GetGridBottomLeft(int Index)
{
    return GetGridCenter(Index) + FVector(-GridSize/2,-GridSize/2,0);
}

void PTGridManager::SetGridState(int Index, EGridState NewState)
{
    if(PTGrids.Num() <= 0)
    {
        return;
    }
    PTGrid GridToSet = PTGrids[Index];
    GridToSet.GridState = NewState;
}

TArray<PTGrid*> PTGridManager::GetGridsFromCenter(int Index, int ARowCount, int AColumnCount)
{
    //todo: add controls for the edges of the map

    
    TArray<PTGrid*> GridsResult;
    if(Index < 0)
    {
        return GridsResult;  
    }
    GridsResult.Reserve(ARowCount*AColumnCount);

    int RowIndex = Index/this->ColumnCount;
    int ColIndex = Index%this->ColumnCount;
    int ColStart = ColIndex - AColumnCount/2;
    int RowStart = RowIndex - ARowCount/2;
    int StartIndex = RowStart*this->ColumnCount + ColStart;

    for(int i = 0; i < AColumnCount; i++)
    {
        for(int j = 0; j < ARowCount; j++)
        {
            GridsResult.Add(&PTGrids[StartIndex + (j*this->ColumnCount) +  i]);                
        }
    }
    return GridsResult; 
}

TArray<PTGrid*> PTGridManager::GetNeighbours(int Index, int ARowCount, int AColumnCount)
{
    TArray<PTGrid*> GridsResult;
    if(Index < 0)
    {
        return GridsResult;  
    }
     GridsResult.Reserve((ARowCount+1)*2 + (AColumnCount+1)*2);

    int RowIndex = Index/this->ColumnCount;
    int ColIndex = Index%this->ColumnCount;
    int ColStart = ColIndex - (AColumnCount+1)/2;
    int RowStart = RowIndex - (ARowCount+1)/2;
    int StartIndex = RowStart*this->ColumnCount + ColStart;


    //Right
    for(int i = 0; i < ARowCount; i++)
    {
        GridsResult.Add(&PTGrids[StartIndex + ColumnCount + AColumnCount + 1 + i*ColumnCount]);
    }
    //Bottom
    for(int i = 0; i < AColumnCount+2; i++)
    {
        GridsResult.Add(&PTGrids[StartIndex+(ColumnCount*(ARowCount+1)) + i]);
    }
    //Left
    for(int i = 0; i < ARowCount; i++)
    {
        GridsResult.Add(&PTGrids[StartIndex + ColumnCount + i*ColumnCount]);
    }
    //Top
    for(int i = 0; i < AColumnCount+2; i++)
    {
        GridsResult.Add(&PTGrids[StartIndex + i]);
    }
    return GridsResult;   
}

FVector PTGridManager::GetPositionToPlace(int Index, int ARowCount, int AColumnCount)
{
    if(ARowCount%2 == 0 && AColumnCount %2 == 0)
    {
        return GetGridTopLeft(Index);
    }
    else if(ARowCount%2 == 0)
    {
        return GetGridTopMid(Index);
    }
    else if(AColumnCount %2 == 0)
    {
        return GetGridLeftMid(Index);
    }
    else
    {
        return GetGridCenter(Index);
    }
}

bool PTGridManager::IsPlaceable(TArray<PTGrid*> GridsToPlace, EGridState RequiredState)
{
    for(int i = 0; i < GridsToPlace.Num(); i++)
    {
        LOG("controlled index %d", GridsToPlace[i]->Index);
        if(GridsToPlace[i] == nullptr || GridsToPlace[i]->GridState != RequiredState)
        {
            LOG("false index %d", GridsToPlace[i]->Index);
            return false;
        }
    }

    return true;
}

void PTGridManager::SetGridStates(TArray<PTGrid*> GridsToSet, EGridState NewState)
{
    for(int i = 0; i < GridsToSet.Num(); i++)
    {
        if(GridsToSet[i] == nullptr)
        {
            continue;
        }
        LOG("Grids set to obstacle %d", i);
        LOG("index %d", GridsToSet[i]->Index);
        GridsToSet[i]->GridState = NewState;
    }
}
