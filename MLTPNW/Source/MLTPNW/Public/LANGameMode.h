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
		struct playerinfor
	{
	public:
		FTransform trans;
		AController* pcontroller;
		playerinfor(AController* pc, FTransform trans=FTransform())
		{
			this->trans = trans;
			pcontroller = pc;
		}
	};
	TMap<int, playerinfor> playerinformap;
	TArray<AActor*> playerstarts;
	TArray<FString> strarray;
	void SplitString(FString str);
	int counter = 0;
	virtual void PostLogin(APlayerController* NewPlayer)override;
	virtual void Logout(AController* Exiting)override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT(""))override;
public:
	// Called when the game starts
	virtual void BeginPlay() override;
	TArray<AActor*> getplayerstarts() { return playerstarts; }
	AActor* getaplayerstart();
};
