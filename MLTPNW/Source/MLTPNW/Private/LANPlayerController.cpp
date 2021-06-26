// Fill out your copyright notice in the Description page of Project Settings.


#include "LANPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "LANGameMode.h"
#define IS_DS
bool ALANPlayerController::b_HumanControlledOnListenserver_ = false;
FString ALANPlayerController::spawnpawnpath = "";
int ALANPlayerController::teamnumber = 0;
int ALANPlayerController::id = 0;

void ALANPlayerController::setspawninfor(const FString& mspawnpawnpath, int mteamnumber)
{
	spawnpawnpath = mspawnpawnpath;
	teamnumber = mteamnumber;
}
void ALANPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (b_HumanControlledOnListenserver)
	{
		b_HumanControlledOnListenserver_ = false;
		b_HumanControlledOnListenserver = false;
	}
}
void ALANPlayerController::BeginPlay()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ALANPlayerController::BeginPlay()"));
	langamemode = Cast<ALANGameMode>(GetWorld()->GetAuthGameMode());
#ifdef IS_DS
	if (!UKismetSystemLibrary::IsDedicatedServer(this))
	{
		SERVER_spawnplayer(spawnpawnpath, teamnumber);
	}
#else
	if (UKismetSystemLibrary::IsServer(this) && !b_HumanControlledOnListenserver_)
	{
		b_HumanControlledOnListenserver_ = true;
		b_HumanControlledOnListenserver = true;
	}
	if (ishumancontrolled() && UKismetSystemLibrary::IsServer(this))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("UKismetSystemLibrary::IsServer(this) true"));
		Async(EAsyncExecution::ThreadPool, [=]() {
			FPlatformProcess::Sleep(0.5);//wait for client????
			AsyncTask(ENamedThreads::GameThread,
				[=]()
				{
					SERVER_spawnplayer(spawnpawnpath, teamnumber);
				}
			);
			}, nullptr);
	}
	if (ishumancontrolled() && !UKismetSystemLibrary::IsServer(this))
	{
		SERVER_spawnplayer(spawnpawnpath, teamnumber);
	}
#endif


	Super::BeginPlay();
}
bool ALANPlayerController::ishumancontrolled()
{
	return b_HumanControlledOnListenserver || GetLocalRole() == ENetRole::ROLE_AutonomousProxy;
}
bool ALANPlayerController::SERVER_spawnplayer_Validate(const FString& str, int tn)
{
	return true;
}
void ALANPlayerController::SERVER_spawnplayer_Implementation(const FString& str, int tn)
{
	UClass* uclass = LoadClass<UObject>(NULL, *str);
	AActor* ps = langamemode->getaplayerstart();
	FTransform transformplane;
    if (ps == nullptr)
	{
	}
	else
	{
		transformplane = ps->GetActorTransform();
		transformplane.SetScale3D(FVector(1));
	}
	if (!restoretrans.Equals(FTransform()))
	{
		transformplane = restoretrans;
	}
	APawn* pawn = GetWorld()->SpawnActorDeferred<APawn>(uclass, transformplane);
	if (pawn)
	{
		UGameplayStatics::FinishSpawningActor(pawn, transformplane);
		UnPossess();
		Possess(pawn);
		mpawn = pawn;
		langamemode->RepossessPawn();
	}
}
void ALANPlayerController::SERVER_Repossesspawn()
{
	UnPossess();
	Possess(mpawn);
}
void ALANPlayerController::SERVER_checkconnection_Implementation()
{
	Client_checkconnection();
}
void ALANPlayerController::Client_checkconnection_Implementation()
{
	bisconnected = true;
	if (TFOnCheckConnectedResult)
	{
		TFOnCheckConnectedResult(1);
	}
}
void ALANPlayerController::checkconnection()
{
	SERVER_checkconnection();
	Async(EAsyncExecution::ThreadPool, [=]() {
		bisconnected = false;
		FPlatformProcess::Sleep(1);
		if (bisconnected)
		{
		}
		else
		{
			TFOnCheckConnectedResult(0);
		}
		}, nullptr);
}
void ALANPlayerController::Client_serverallotid_Implementation(int idp)
{
	ALANPlayerController::id = idp;

}