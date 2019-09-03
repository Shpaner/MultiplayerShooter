
#include "DynamicLight.h"
#include "Components/PointLightComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "CoopGameGameMode.h"



ADynamicLight::ADynamicLight()
{
	PrimaryActorTick.bCanEverTick = true;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	RootComponent = PointLight;

	DefaultSeconds = 15;
	SecondsLeft = DefaultSeconds;

	static ConstructorHelpers::FObjectFinder<USoundBase> SwitchSound(TEXT("SoundWave'/Game/Audio/Energy.Energy'"));
	static ConstructorHelpers::FObjectFinder<USoundBase> FailSound(TEXT("SoundWave'/Game/Audio/lightswitchfail.lightswitchfail'"));
	static ConstructorHelpers::FObjectFinder<USoundBase> WSound(TEXT("SoundWave'/Game/Audio/WinSound.WinSound'"));

	LightSwitchSound = SwitchSound.Object;
	LightSwitchFailSound = FailSound.Object;
	WinSound = WSound.Object;

	BlueTeamDbgMessage = FString::Printf(TEXT("BLUE TEAM IS WINNING: "));
	RedTeamDbgMessage = FString::Printf(TEXT("RED TEAM IS WINNING: "));
}


void ADynamicLight::BeginPlay()
{
	DisplayScore();
}



void ADynamicLight::DisplayScore()
{
		ACoopGameGameMode* GM = Cast<ACoopGameGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->DisplayScore();
		}
}



void ADynamicLight::ChangeLightToRed()
{
	if (!bRedLightIsOn)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LightSwitchSound, GetActorLocation());

		bBlueLightIsOn = false;

		bRedLightIsOn = true;

		PointLight->SetLightColor(FColor::Red);

		SecondsLeft = DefaultSeconds;

		GetWorldTimerManager().ClearTimer(BlueTimeHandle);

		GetWorldTimerManager().SetTimer(RedTimeHandle, this, &ADynamicLight::DisplayRedTimer, 1.0f, true, 0.0f);
	}

	else if (bRedLightIsOn)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LightSwitchFailSound, GetActorLocation());
	}
}


void ADynamicLight::ChangeLightToBlue()
{
	if (!bBlueLightIsOn)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LightSwitchSound, GetActorLocation());

		bRedLightIsOn = false;

		bBlueLightIsOn = true;

		PointLight->SetLightColor(FColor::Blue);

		SecondsLeft = DefaultSeconds;

		GetWorldTimerManager().ClearTimer(RedTimeHandle);

		GetWorldTimerManager().SetTimer(BlueTimeHandle, this, &ADynamicLight::DisplayBlueTimer, 1.0f, true, 0.0f);
	}

	else if(bBlueLightIsOn)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LightSwitchFailSound, GetActorLocation());
	}
}



void ADynamicLight::DisplayRedTimer()
{
	if (!bGameOver)
	{
		if (GEngine)
		{
			if (SecondsLeft > 0)
			{
				GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Red, RedTeamDbgMessage + FString::FromInt(SecondsLeft), false, FVector2D(3, 3));
				SecondsLeft--;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, TEXT("RED TEAM WIN!"), false, FVector2D(3, 3));

				EndRound();
			}
		}

		
	}
}


void ADynamicLight::DisplayBlueTimer()
{
	if (!bGameOver)
	{
		if (GEngine)
		{
			if (SecondsLeft > 0)
			{
				GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Blue, BlueTeamDbgMessage + FString::FromInt(SecondsLeft), false, FVector2D(3, 3));
				SecondsLeft--;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Blue, TEXT("BLUE TEAM WIN!"), false, FVector2D(3, 3));
			
				EndRound();
			}
		}
	}
}




void ADynamicLight::EndRound()
{
	GetWorldTimerManager().ClearTimer(BlueTimeHandle);
	GetWorldTimerManager().ClearTimer(RedTimeHandle);

	PointLight->SetLightColor(FColor::White);

	ACoopGameGameMode* GM = Cast<ACoopGameGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{

		SecondsLeft = 0;

		if (bRedLightIsOn)
		{
			GM->RedTeamScore += 3;
		}

		else if (bBlueLightIsOn)
		{
			GM->BlueTeamScore += 3;
		}
	}

	bRedLightIsOn = false;
	bBlueLightIsOn = false;

	UGameplayStatics::PlaySound2D(this, WinSound);

	DisplayScore();
}



