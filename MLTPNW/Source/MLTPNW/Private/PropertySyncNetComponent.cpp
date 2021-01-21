// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertySyncNetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
bool UPropertySyncNetComponent::b_HumanControlledOnListenserver_ = false;
// Sets default values for this component's properties
void UPropertySyncNetComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UPropertySyncNetComponent, syncproperty);
}
UPropertySyncNetComponent::UPropertySyncNetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UPropertySyncNetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (b_HumanControlledOnListenserver)
	{
		b_HumanControlledOnListenserver_ = false;
		b_HumanControlledOnListenserver = false;
	}
}
void UPropertySyncNetComponent::BeginPlay()
{
	if (UKismetSystemLibrary::IsServer(this)&& !b_HumanControlledOnListenserver_)
	{
		b_HumanControlledOnListenserver_ = true;
		b_HumanControlledOnListenserver = true;
	}
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPropertySyncNetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
void UPropertySyncNetComponent::setproperty(const FString& str)
{
	SERVER_Setsyncproperty(str);
}
bool UPropertySyncNetComponent::SERVER_Setsyncproperty_Validate(const FString& str)
{
	return true;
}
void UPropertySyncNetComponent::SERVER_Setsyncproperty_Implementation(const FString& str)
{
	//Rep
	syncproperty = str;
	OnRep_syncproperty();
}
void UPropertySyncNetComponent::OnRep_syncproperty()
{
	Async(EAsyncExecution::ThreadPool, [=]() {
		FPlatformProcess::Sleep(0.5f);
		AsyncTask(ENamedThreads::GameThread,
			[=]()
			{
				onpropertychangeevent.Broadcast(syncproperty);
				onpropertychangeeventv1.ExecuteIfBound(syncproperty);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, syncproperty);
			}
		);
	}, nullptr);
}

bool UPropertySyncNetComponent::ishumancontrolled()
{
	return b_HumanControlledOnListenserver || GetOwnerRole()== ENetRole::ROLE_AutonomousProxy;
}

