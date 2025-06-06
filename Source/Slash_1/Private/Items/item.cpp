// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/item.h"
#include "Slash_1/DebugMacros.h"
#include "Slash_1/Slash_1.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PickupInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
Aitem::Aitem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void Aitem::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &Aitem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &Aitem::OnSphereEndOverlap);
}

float Aitem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float Aitem::TransformedCos()
{
	return  Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void Aitem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface) {
		PickupInterface->SetOverlappingItem(this);
	}
}

void Aitem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface) {
		PickupInterface->SetOverlappingItem(this);
	}
}

void Aitem::SpawnPickupSystem()
{
	if (PickupEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation()
		);
	}
}

void Aitem::SpawnPickupSound()
{
	if (PickupSound) {
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			PickupSound,
			GetActorLocation()
		);
	}
}

// Called every frame
void Aitem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering) {
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
	

	//UE_LOG(LogTemp, Warning, TEXT("DeltaTime: %f"), DeltaTime);
	//if (GEngine) {
	//	FString Name = GetName();
	//	FString Message = FString::Printf(TEXT("Item Name: %s"), *Name);
	//	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, Message);
	//	UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *Name);
	//}

	
	/*float MovementRate = 50.f;
	float RotationRate = 45.f;
	AddActorWorldOffset(FVector(MovementRate*DeltaTime, 0.f, 0.f));
	AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));*/

	
	
	/*DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);*/

}

