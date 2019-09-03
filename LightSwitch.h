

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightSwitch.generated.h"

class UBoxComponent;
class ADynamicLight;

UCLASS()
class COOPGAME_API ALightSwitch : public AActor
{
	GENERATED_BODY()
	
public:	

	ALightSwitch();

protected:

	virtual void BeginPlay() override;

	UBoxComponent* OverlapComp;

	UDecalComponent* DecalComp;

	UPROPERTY(EditAnywhere)
		AActor* PointLightSpawner;

	FVector Location;

	ADynamicLight* PointLight;

	UFUNCTION()
		void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
