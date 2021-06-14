// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LANPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MLTPNW_API ALANPlayerController : public APlayerController
{
	GENERATED_BODY()
	static bool b_HumanControlledOnListenserver_;
	bool b_HumanControlledOnListenserver = false;
	class ALANGameMode* langamemode;
	static	FString spawnpawnpath;
	static int teamnumber;
	static int id;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;
	UFUNCTION(reliable, server)
		void SERVER_checkconnection();
	UFUNCTION(reliable, Client)
		void Client_checkconnection();
	bool bisconnected = false;
public:
	APawn* mpawn;
	int static getid() { return ALANPlayerController::id; }
	FTransform restoretrans;
	UFUNCTION(BlueprintCallable)
	static void setspawninfor(const FString & mspawnpawnpath, int mteamnumber=0);
	UFUNCTION(reliable, server, WithValidation)
	void SERVER_spawnplayer(const FString& str,int tn);
	UFUNCTION(reliable, client)
		void Client_serverallotid(int idp);
	UFUNCTION(BlueprintCallable)
		void checkconnection();

	TFunction<void(uint8 msg)> TFOnCheckConnectedResult;

	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintPure)
	bool ishumancontrolled();
};
