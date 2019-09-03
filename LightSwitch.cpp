

#include "LightSwitch.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "SCharacterPRO.h"
#include "DynamicLight.h"



ALightSwitch::ALightSwitch()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(100.0f));
	RootComponent = OverlapComp;

	OverlapComp->SetHiddenInGame(false);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ALightSwitch::HandleOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(100.0f);
	DecalComp->SetupAttachment(RootComponent);

	SetReplicates(true);
}



void ALightSwitch::BeginPlay()
{
	if (PointLightSpawner != nullptr)
	{
		Location = PointLightSpawner->GetActorLocation();
		PointLight = (ADynamicLight*)GetWorld()->SpawnActor(ADynamicLight::StaticClass(), &Location);
	}
}

void ALightSwitch::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASCharacterPRO* MyPawn = Cast<ASCharacterPRO>(OtherActor);
	
	if (MyPawn != nullptr && PointLight)
	{

			if (MyPawn->bIsInTeamBlue)
			{
				PointLight->ChangeLightToBlue();
			}

			else if (MyPawn->bIsInTeamRed)
			{
				PointLight->ChangeLightToRed();
			}
	}

	else
	{
		return;
	}
}
