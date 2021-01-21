// Fill out your copyright notice in the Description page of Project Settings.


#include "LANWheeledVehicle.h"
#include "PropertySyncNetComponent.h"
#include "Kismet/GameplayStatics.h"

ALANWheeledVehicle::ALANWheeledVehicle()
{
	PrimaryActorTick.bCanEverTick = true;
	PropertySyncNetCom = CreateDefaultSubobject<UPropertySyncNetComponent>(TEXT("PropertySyncNetCom"));
	PropertySyncNetCom->SetupAttachment(GetRootComponent());
	PropertySyncNetCom->SetRelativeLocation(FVector(0,0,0)); // Position the camera
}


// Called when the game starts or when spawned
void ALANWheeledVehicle::BeginPlay()
{
	Super::BeginPlay();
	PropertySyncNetCom->onpropertychangeeventv1.BindLambda([=](const FString& str) {

		});
	if (PropertySyncNetCom->ishumancontrolled())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("PropertySyncNetCom->ishumancontrolled()"));

		PropertySyncNetCom->setproperty(FString::FromInt(FMath::Rand()));
		UGameplayStatics::GetPlayerController(this, 0)->UnPossess();
		UGameplayStatics::GetPlayerController(this, 0)->Possess(this);
	}


}

// Called every frame
void ALANWheeledVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}