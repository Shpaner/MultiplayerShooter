// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootingRangeTarget.generated.h"

class UHealthrComponent;
class UParticleSystem;
class URadialForceComponent;
class USoundBase;
class UStaticMeshComponent;
class UMaterial;

UCLASS()
class COOPGAME_API AShootingRangeTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	AShootingRangeTarget();

protected:

	UStaticMeshComponent* MeshComp;

	UHealthrComponent* HealthComp;

	URadialForceComponent* RadialForceComp;

	UFUNCTION()
	void OnHealthChanged(UHealthrComponent* OwningHealthComp, float Health, float HealthDelta, 
		const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UParticleSystem* ExplosionEffect;

	USoundBase* ExplosionSound;

	float ExplosionImpulse;

	UMaterialInterface* ExplodedMaterial;

	UPROPERTY(ReplicatedUsing = OnRep_Exploded)
		bool bExploded;

	UFUNCTION()
		void OnRep_Exploded();
};
