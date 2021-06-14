// Fill out your copyright notice in the Description page of Project Settings.


#include "LANGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "LANPlayerController.h"

void ALANGameMode::SplitString(FString str)
{//////?xxxxx?xxxxx?xxxxx...
	FString str1;
	FString str2;
	str.Split("?", &str1, &str2, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	strarray.Insert(str2, 0);
	if (!str1.IsEmpty())
	{
		SplitString(str1);
	}
}
void ALANGameMode::BeginPlay()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ALANGameMode::BeginPlay()"));
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), playerstarts);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(playerstarts.Num()));

}
AActor* ALANGameMode::getaplayerstart()
{
	if (playerstarts.Num() == 0)
	{
		return nullptr;
	}
	AActor* temp = playerstarts[counter++];
	if (counter == playerstarts.Num())
	{
		counter = 0;
	}
	return temp;
}
void ALANGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("PostLogin"));

}
void ALANGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Logout"));
	for (int i = 1; i <= playerinformap.Num(); i++)
	{
		ALANGameMode::playerinfor* pi = playerinformap.Find(i);
		if (pi)
		{
			if (pi->pcontroller == Exiting)
			{
				ALANPlayerController* pc = Cast<ALANPlayerController>(Exiting);
				if (pc)
				{
					pi->trans = pc->mpawn->GetTransform();
				}
				break;
			}
		}
	}
}
FString ALANGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString restr = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	SplitString(Options);
	int tempid = FCString::Atoi(*strarray[0]);
	ALANPlayerController* pc = Cast<ALANPlayerController>(NewPlayerController);
	if (tempid == 0)
	{
		int tid = playerinformap.Num() + 1;
		playerinformap.Add(tid, ALANGameMode::playerinfor(NewPlayerController));
		if (pc)
		{
			Async(EAsyncExecution::ThreadPool, [=]() {
				FPlatformProcess::Sleep(0.2);//wait for client????
				AsyncTask(ENamedThreads::GameThread,
					[=]()
					{
						pc->Client_serverallotid(tid);
					}
				);
				}, nullptr);
		}
	}
	else
	{
		ALANGameMode::playerinfor* pi = playerinformap.Find(tempid);
		if (pi)
		{
			pi->pcontroller = NewPlayerController;
			if (pc)
			{
				pc->restoretrans = pi->trans;
			}
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("This is yellow text!"));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("InitNewPlayer"));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Options);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, restr);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, *strarray[0]);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, *strarray[1]);
	return restr;
}