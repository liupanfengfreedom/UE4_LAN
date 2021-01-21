// Fill out your copyright notice in the Description page of Project Settings.


#include "LANPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "LANGameMode.h"

bool ALANPlayerController::b_HumanControlledOnListenserver_ = false;
FString ALANPlayerController::spawnpawnpath = "";
int ALANPlayerController::teamnumber = 0;
void ALANPlayerController::BeginPlay()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ALANPlayerController::BeginPlay()"));
	langamemode = Cast<ALANGameMode>(GetWorld()->GetAuthGameMode());
	if (UKismetSystemLibrary::IsServer(this) && !b_HumanControlledOnListenserver_)
	{
		b_HumanControlledOnListenserver_ = true;
		b_HumanControlledOnListenserver = true;
	}
	if (ishumancontrolled() && UKismetSystemLibrary::IsServer(this))
	{
		Async(EAsyncExecution::ThreadPool, [=]() {
			FPlatformProcess::Sleep(0.2);
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
	}
	APawn* pawn = GetWorld()->SpawnActorDeferred<APawn>(uclass, transformplane);
	if (pawn)
	{
		UGameplayStatics::FinishSpawningActor(pawn, transformplane);
		UnPossess();
		Possess(pawn);
	}
}