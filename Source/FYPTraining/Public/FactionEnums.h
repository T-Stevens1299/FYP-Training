// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FactionEnums.generated.h"

UENUM(BlueprintType, Category = "Faction Assingment")
enum class EFactionTag : uint8
{
	HumanPlayer UMETA(DisplayName = "Human Player"),
	AiPlayer1 UMETA(DisplayName = "AI Player 1"),
	AiPlayer2 UMETA(DisplayName = "AI Player 2"),
};