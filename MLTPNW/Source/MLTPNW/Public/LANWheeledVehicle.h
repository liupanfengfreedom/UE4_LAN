// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "LANWheeledVehicle.generated.h"

/**
 * 
 */
UCLASS()
class MLTPNW_API ALANWheeledVehicle : public AWheeledVehicle
{
	GENERATED_BODY()
public:
		UPROPERTY(Category = Vehicle, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPropertySyncNetComponent* PropertySyncNetCom;
protected:
	ALANWheeledVehicle();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
