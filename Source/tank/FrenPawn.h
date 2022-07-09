// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ABasicClass;
class TANK_API FrenPawn
{
public:
	static void AddPawn(ABasicClass* AddablePawn);

	static void DelPawn(ABasicClass* DeletablePawn);


	static TMap<double, ABasicClass*> GetUnFriendly(ABasicClass* ThisPawn);

protected:

	UPROPERTY()
	static TArray<ABasicClass*> BasePawns;
};
