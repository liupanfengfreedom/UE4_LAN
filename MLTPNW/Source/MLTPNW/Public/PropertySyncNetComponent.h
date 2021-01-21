// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PropertySyncNetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MLTPNW_API UPropertySyncNetComponent : public USceneComponent
{
	GENERATED_BODY()
		static bool b_HumanControlledOnListenserver_;
	bool b_HumanControlledOnListenserver = false;
public:	
	// Sets default values for this component's properties
	UPropertySyncNetComponent();
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;
public:	
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION(reliable, server, WithValidation)
		void SERVER_Setsyncproperty(const FString& str);
	UPROPERTY(ReplicatedUsing = OnRep_syncproperty)
		FString syncproperty;
	UFUNCTION()
		virtual void OnRep_syncproperty();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
		bool ishumancontrolled();
	UFUNCTION(BlueprintCallable)
		void setproperty(const FString& str);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPropertychange, const FString&, str);
	DECLARE_DELEGATE_OneParam(FOnPropertychangev1, const FString&);
	UPROPERTY(BlueprintAssignable)
	FOnPropertychange onpropertychangeevent;
	FOnPropertychangev1 onpropertychangeeventv1;
		/** Marks the properties we wish to replicate */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
