
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicLight.generated.h"

class UPointLightComponent;

UCLASS()
class COOPGAME_API ADynamicLight : public AActor
{
	GENERATED_BODY()
	
public:	

	ADynamicLight();

	UPROPERTY(EditAnywhere)
		UPointLightComponent* PointLight;

	void ChangeLightToRed();

	void ChangeLightToBlue();

	int SecondsLeft;

protected:

	virtual void BeginPlay() override;

	void DisplayRedTimer();

	void DisplayBlueTimer();

	bool bRedLightIsOn;

	bool bBlueLightIsOn;

	FTimerHandle RedTimeHandle;

	FTimerHandle BlueTimeHandle;

	USoundBase* LightSwitchSound;

	USoundBase* LightSwitchFailSound;

	USoundBase* WinSound;

	int DefaultSeconds;

	bool bGameOver;

	FString BlueTeamDbgMessage;

	FString RedTeamDbgMessage;

	void EndRound();

	void DisplayScore();
};
