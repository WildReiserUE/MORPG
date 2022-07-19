// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMeshComponent->SetGenerateOverlapEvents(true);
	SetRootComponent(ItemMeshComponent);
	//ItemMeshComponent->OnClicked.AddUniqueDynamic(this, &ATDSItemBase::RenderOn);
}

void AItemBase::SpawnParticleFx(UParticleSystem* NewParticle)
{
	if(!GetWorld() || !NewParticle) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NewParticle, GetOwner()->GetActorLocation());
}

void AItemBase::RenderOn(UPrimitiveComponent* pComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, TEXT("Item: Switch RenderON"));
	UE_LOG(LogViewport, Display, TEXT("Command to RENDER ON"));
	if(pComponent)
	{
		pComponent->SetRenderCustomDepth(true);
	}
}

void AItemBase::RenderOff(UPrimitiveComponent* pComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, TEXT("Item: Switch RenderOFF"));
	UE_LOG(LogViewport, Display, TEXT("Command to RENDER OFF"));
	if (pComponent)
	{
		pComponent->SetRenderCustomDepth(false);
	}
}

void AItemBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	ChangeSettings();
}

void AItemBase::ChangeSettings()
{
	if(ItemInfo.ItemMesh)
	{
		ItemMeshComponent->SetStaticMesh(ItemInfo.ItemMesh);
		ItemMeshComponent->OnBeginCursorOver.AddUniqueDynamic(this, &AItemBase::RenderOn);
		ItemMeshComponent->OnEndCursorOver.AddUniqueDynamic(this, &AItemBase::RenderOff);
	}
	else
		ItemMeshComponent->SetStaticMesh(nullptr);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

