// Fill out your copyright notice in the Description page of Project Settings.


#include "PTGrid.h"

PTGrid::PTGrid(int ArrayIndex, EGridState NewState)
{
    Index = ArrayIndex;
    GridState = NewState;
}

PTGrid::~PTGrid()
{
}
