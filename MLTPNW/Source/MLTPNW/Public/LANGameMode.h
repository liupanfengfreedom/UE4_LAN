// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LANGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MLTPNW_API ALANGameMode : public AGameModeBase
{
	GENERATED_BODY()
	TArray<AActor*> playerstarts;
	int counter = 0;
public:
	// Called when the game starts
	virtual void BeginPlay() override;
	TArray<AActor*> getplayerstarts() { return playerstarts; }
	AActor* getaplayerstart();
};
