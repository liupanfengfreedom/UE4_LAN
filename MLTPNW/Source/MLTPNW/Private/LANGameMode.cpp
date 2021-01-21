// Fill out your copyright notice in the Description page of Project Settings.


#include "LANGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

void ALANGameMode::BeginPlay()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ALANGameMode::BeginPlay()"));
	UGameplayStatics::GetAllActorsOfClass(this,APlayerStart::StaticClass(),playerstarts);
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