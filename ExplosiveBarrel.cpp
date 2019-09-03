
#include "ShootingRangeTarget.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/CoreUObject/Public/UObject/Object.h"
#include "HealthrComponent.h"
#include "Runtime/Engine/Classes/PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystem.h"
#include "ConstructorHelpers.h"


AShootingRangeTarget::AShootingRangeTarget()
{
	PrimaryActorTick.bCanEverTick = true;

	//Mesh & Material
	MeshComp = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Meshes/SM_Barrel01.SM_Barrel01'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultMaterial(TEXT("MaterialInstanceConstant'/Game/Meshes/MI_Barrel01_1.MI_Barrel01_1'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> SecondMaterial(TEXT("Material'/Game/ExplosiveBarrel/M_ExplosiveBarrelExploded.M_ExplosiveBarrelExploded'"));

	ExplodedMaterial = SecondMaterial.Object;

	MeshComp->SetStaticMesh(StaticMesh.Object);
	MeshComp->SetMaterial(0, DefaultMaterial.Object);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);

	RootComponent = MeshComp;

	//Damage and Explosion Effects
	HealthComp = CreateDefaultSubobject<UHealthrComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &AShootingRangeTarget::OnHealthChanged);

	RadialForceComp = CreateAbstractDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->Radius = 250;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;

	ExplosionImpulse = 450;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(TEXT("ParticleSystem'/Game/WeaponEffects/Explosion/P_Explosion.P_Explosion'"));
	static ConstructorHelpers::FObjectFinder<USoundBase> ExpSound(TEXT("SoundWave'/Game/TrackerBall/Explosion01.Explosion01'"));

	ExplosionEffect = Particle.Object;
	ExplosionSound = ExpSound.Object;

	//Multiplayer
	SetReplicates(true);
	SetReplicateMovement(true);
}


void AShootingRangeTarget::OnHealthChanged(UHealthrComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bExploded)
	{
		return;
	}

	if (Health <= 0.0f)
	{
		bExploded = true;
		OnRep_Exploded();

		FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
		MeshComp->AddImpulse(BoostIntensity, NAME_None, true);

		RadialForceComp->FireImpulse();

	}
}


void AShootingRangeTarget::OnRep_Exploded()
{
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 100.0f, GetActorLocation(), 250, StaticClass(), TArray<AActor*>(), this, (AController*)GetOwner(), true, ECollisionChannel::ECC_Pawn);
	
	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}
	
	MeshComp->SetMaterial(0, ExplodedMaterial);
}


void AShootingRangeTarget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShootingRangeTarget, bExploded);
}