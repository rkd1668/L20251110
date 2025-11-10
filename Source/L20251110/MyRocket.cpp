// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRocket.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMyRocket::AMyRocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(30.f, 11.f, 11.f));

	Rocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket"));
	Rocket->SetupAttachment(Box);
	Rocket->SetRelativeRotation(FRotator(-90.f, 0, 0));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/SM_Rocket.SM_Rocket'"));
	if (SM_Body.Succeeded())
	{
		Rocket->SetStaticMesh(SM_Body.Object);
	}

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 2000.0f;
	Movement->MaxSpeed = 2000.f;
	Movement->ProjectileGravityScale = 0;

}

// Called when the game starts or when spawned
void AMyRocket::BeginPlay()
{
	Super::BeginPlay();
	
	//SetLifeSpan(3.f);

	FLatentActionInfo Info;
	Info.ExecutionFunction = TEXT("Timeout");
	Info.CallbackTarget = this;
	Info.Linkage = 1;
	Info.UUID = 0;

	UKismetSystemLibrary::Delay(GetWorld(), 1.0f, Info);

	//Delegate UE
	OnActorBeginOverlap.AddDynamic(this, &AMyRocket::ProcessActorBeginOverlap);
}

// Called every frame
void AMyRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyRocket::Timeout()
{
	//UE_LOG(LogTemp, Warning, TEXT("Die"));
	Destroy();
}

void AMyRocket::ProcessActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

	UE_LOG(LogTemp, Warning, TEXT("ProcessActorBeginOverlap"));

	UGameplayStatics::ApplyDamage(OtherActor,
		10.0f,
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		this,
		nullptr
	);
}

