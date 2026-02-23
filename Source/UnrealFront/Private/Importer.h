// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. See LICENSE in the root of this repository or https://www.gnu.org/licenses/
#pragma once
#include "CoreMinimal.h"
//#include "Blueprint/UserWidget.h"
#include "ProcStaticMeshActor.h"
#include "Structs.h"
#include <LibSWBF2/LibSWBF2.h>

#include "Importer.generated.h"


UCLASS(BlueprintType)
class UImporter : public UObject //, public FTickableGameObject
{
	GENERATED_UCLASS_BODY()

public:
	~UImporter();

	UFUNCTION(BlueprintCallable)
	UTexture2D* ImportTexture(const FString& textureName);

	UFUNCTION(BlueprintCallable)
	USoundWave* ImportSound(const FString& soundName);

	UFUNCTION(BlueprintCallable)
	bool GetLocalizedString(const FString& language, const FString& localizePath, FString& outLocalized);

public:
	LibSWBF2::Handle AddLevel(const FString& path, const TSet<FString>* subLVLFilter=nullptr);
	LibSWBF2::Handle AddSoundBank(const FString& path);
	void StartLoading();
	void FreeAll();
	bool IsDone() const;
	TSet<LibSWBF2::Handle> GetLoadedLevels() const;
	LibSWBF2::ELoadStatus GetLevelStatus(LibSWBF2::Handle handle) const;
	float_t GetLevelProgress(LibSWBF2::Handle handle) const;
	LibSWBF2::Level* GetLevel(LibSWBF2::Handle handle) const;
	float_t GetOverallProgress() const;

	//bool ImportTo(ULevel* spawnLevel, const FMaterialImportSettings& materialSettings);

	//virtual void Tick(float DeltaTime) override;
	//virtual bool IsTickable() const override;
	//virtual TStatId GetStatId() const override;

	UTexture2D* ImportTexture(const LibSWBF2::Texture& lvlTexture);
	//ALight* ImportLight(const LibSWBF2::Light& light, ULevel* spawnLevel);
	AProcStaticMeshActor* ImportModel(const LibSWBF2::Model& model, const FModelImportSettings& modelSettings, const FMaterialImportSettings& materialSettings, ULevel* spawnLevel, FString nameOverride="");
	//ASkeletalMeshActor* ImportSkeletalMesh(const LibSWBF2::Model& model, const FModelImportSettings& modelSettings, const FMaterialImportSettings& materialSettings);
	AProcStaticMeshActor* ImportTerrain(const LibSWBF2::Terrain& terrain, bool bEnsureImportOnce, const FMaterialImportSettings& materialSettings, ULevel* spawnLevel);
	USoundWave* ImportSound(const LibSWBF2::Sound& sound);
	//void ImportWorld(const LibSWBF2::World& lvlWorld, const FMaterialImportSettings& materialSettings, ULevel* spawnLevel);

	bool ExecuteLUA(const FString& scriptName);
	bool ExecuteLUA(LibSWBF2::Handle lvlHandle, const FString& scriptName);

private:
	LibSWBF2::Container* Container;

	TSet<FString> LoadedTerrains;
	TMap<FString, AProcStaticMeshActor*> ModelCache;
	TMap<FString, UTexture2D*> TextureCache;
	TMap<LibSWBF2::FNVHash, USoundWave*> SoundCache;

private:
	FVector3f ToUnreal(const LibSWBF2::Vector3& vector);
	FVector2f ToUnreal(const LibSWBF2::Vector2& vector);
	FQuat ToUnreal(const LibSWBF2::Vector4& quaternion);
	FColor ToUnreal(const LibSWBF2::Color4u8& color);
	FLinearColor ToLinearColor(const LibSWBF2::Color4f& color); 
	TArray<FVector3f> ConvertVertexBuffer(uint32 count, LibSWBF2::Vector3* buffer);
	TArray<FVector3f> ConvertVertexBuffer(uint32 count, LibSWBF2::Vector3* buffer, float multiplicator);
	//FRuntimeMeshCollisionVertexStream ConvertVertexBufferRMC(uint32 count, LibSWBF2::Vector3* buffer, float multiplicator);
	TArray<int32> ConvertIndexBuffer(uint32 count, uint16* buffer);
	TArray<int32> ConvertIndexBuffer(uint32 count, uint32* buffer);
	//FRuntimeMeshCollisionTriangleStream ConvertIndexBufferRMC(uint32 count, uint32* buffer);
	TArray<FVector2f> ConvertUVBuffer(uint32 count, LibSWBF2::Vector2* buffer);
	TArray<FColor> ConvertColorBuffer(uint32 count, LibSWBF2::Color4u8* buffer);
};

//UCLASS(BlueprintType)
//class ULoadscreenWidget : public UUserWidget
//{
//	GENERATED_UCLASS_BODY()
//
//public:
//	UPROPERTY(BlueprintReadWrite)
//	UWidgetAnimation* BlendInAnimation;
//
//	UPROPERTY(BlueprintReadWrite)
//	UWidgetAnimation* BlendOutAnimation;
//
//	// every derived BP needs to implement this and set BlendInAnimation and 
//	// BlendOutAnimation accordingly!
//	UFUNCTION(BlueprintImplementableEvent)
//	void SetAnimations();
//};

