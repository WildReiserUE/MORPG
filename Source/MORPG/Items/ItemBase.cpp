// Created WildReiser


#include "ItemBase.h"
#include "Kismet/GameplayStatics.h"


AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMeshComponent->SetGenerateOverlapEvents(true);
	SetRootComponent(ItemMeshComponent);
	ItemMeshComponent->SetMobility(EComponentMobility::Movable);
	ItemWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemWidget"));
	ItemWidgetComponent->SetupAttachment(ItemMeshComponent);
}

void AItemBase::SpawnParticleFx(UParticleSystem* NewParticle)
{
	if(!GetWorld() || !NewParticle) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NewParticle, GetOwner()->GetActorLocation());
}

void AItemBase::RenderOff(UPrimitiveComponent* pComponent)
{	
	if (pComponent && !bClicked)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, TEXT("Item: Switch RenderOFF"));
		UE_LOG(LogViewport, Display, TEXT("Command to RENDER OFF"));
		pComponent->SetRenderCustomDepth(false);
	}
}

void AItemBase::RenderLock()
{
	if(ItemMeshComponent && !bClicked)
	{
		bClicked = true;
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, TEXT("Item: Switch RenderLOCK"));
		UE_LOG(LogViewport, Display, TEXT("Command to RENDER LOCK"));
		ItemMeshComponent->SetRenderCustomDepth(true);
	}
	else if (bClicked)
	{
		bClicked = false;
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, TEXT("Item: Switch RenderOFF"));
		UE_LOG(LogViewport, Display, TEXT("Command to RENDER OFF"));
		ItemMeshComponent->SetRenderCustomDepth(false);
	}
}

void AItemBase::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	RenderLock();
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
		ItemMeshComponent->CanCharacterStepUpOn;
		ItemMeshComponent->SetEnableGravity(true);
		ItemMeshComponent->SetSimulatePhysics(true);
		ItemMeshComponent->SetCanEverAffectNavigation(false);
		ItemMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
		ItemMeshComponent->OnEndCursorOver.AddUniqueDynamic(this, &AItemBase::RenderOff);
		ItemMeshComponent->OnBeginCursorOver.AddUniqueDynamic(this, &AItemBase::ShowItemName);
	}
	else
		ItemMeshComponent->SetStaticMesh(nullptr);
}

void AItemBase::ShowItemName(UPrimitiveComponent* pComponent)
{
	if(pComponent)
	{
		UE_LOG(LogViewport, Display, TEXT("Command to SHOW WIDGET"));
		ItemMeshComponent->SetRenderCustomDepth(true);
	}
}

void AItemBase::BeginPlay()
{
	//ChangeSettings();
}
