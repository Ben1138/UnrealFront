// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. See LICENSE in the root of this repository or https://www.gnu.org/licenses/
#include "ProcStaticMeshActor.h"
#include "UnrealFront/UnrealFront.h"

// Sets default values
AProcStaticMeshActor::AProcStaticMeshActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<URealtimeMeshComponent>(TEXT("RealtimeMeshComponent"));
	MeshComp->SetMobility(EComponentMobility::Static);
	MeshComp->SetGenerateOverlapEvents(false);
	MeshComp->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	SetRootComponent(MeshComp);
}

// Called when the game starts or when spawned
//void AProcStaticMeshActor::BeginPlay()
//{
//	Super::BeginPlay();
//}

// Called every frame
//void AProcStaticMeshActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

