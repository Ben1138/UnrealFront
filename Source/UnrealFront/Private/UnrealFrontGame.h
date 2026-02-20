// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. See LICENSE in the root of this repository or https://www.gnu.org/licenses/
#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Importer.h"
#include "Structs.h"
#include <LibSWBF2/LibSWBF2.h>

#include "UnrealFrontGame.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoArgsDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMapSwitchFinishedDelegate, float, Duration);


UCLASS()
class UUnrealFrontGame : public UGameInstance, public FTickableGameObject
{
public:
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadWrite)
	FMaterialImportSettings MaterialSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "UnrealFront")
		FString GameDataPath;

	UFUNCTION(BlueprintCallable)
		bool VerifyGameDataPath();

private:
	static UUnrealFrontGame* GameInstance;

	UPROPERTY()
		UImporter* Importer = nullptr;

	UPROPERTY()
		ULevel* MainMenu;

	// Loadscreen to use in the next map switch
	//UPROPERTY()
	//	ULoadscreenWidget* LoadscreenBP;

	// loadscreen currently in use during map switch
	//UPROPERTY()
	//	ULoadscreenWidget* CurrentLoadscreen;

	LibSWBF2::Handle LoadscreenLVL = SWBF2HANDLE_INVALID;

	bool bLoadingShellLVL;
	bool bIsSwitchingMaps;
	bool bLoadscreenLoadedEventFired;
	bool bUELevelLoaded;
	bool bLVLWorldsImported;
	bool bMapSwitchFinishedEventFired;
	bool bLoadscreenFadeInFinished;
	bool bCustomLoadscreenLoaded;

	float MapSwitchStartedTime;
	FName CurrentUnrealLevel;
	float LoadscreenMinDisplayTime;

	// Maps lua scripts (TESc_con) to specific Addon maps (TES)
	TMap<FString, FString> DownloadableContents;
	FString DLCEnvironmentPath;	// NO subsequent "\\"

	TArray<FString> MapRotation;
	int32 CurrentMapRotationIndex = -1;

	TSet<LibSWBF2::Handle> AddmeScriptHandles;

private:
	static bool LVLExists(const FString& lvlPath);

public:
	void Init() override;
	void Shutdown() override;
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

	static LibSWBF2::FNVHash FNVHash(const FString& str);

	UFUNCTION(BlueprintCallable)
		static UUnrealFrontGame* GetGameInstance();

	UPROPERTY(BlueprintAssignable)
		FNoArgsDelegate OnLoadTextureFinished;

	UPROPERTY(BlueprintAssignable)
		FNoArgsDelegate OnMapSwitch;

	UPROPERTY(BlueprintAssignable)
		FMapSwitchFinishedDelegate OnMapSwitchFinished;

	UPROPERTY(BlueprintAssignable)
		FNoArgsDelegate OnShellLVLFinished;


	UFUNCTION(BlueprintCallable)
		void OnUELevelLoaded();

	static LibSWBF2::Handle AddLVL(FString lvlPath, const TSet<FString>* subLVLFilter=nullptr, bool bEssential=false);
	static LibSWBF2::Handle AddSoundBank(FString soundBankPath);

	UFUNCTION(BlueprintCallable)
		static UWorld* GetCurrentWorld();

	UFUNCTION(BlueprintCallable)
		static bool IsEditor();

	//UFUNCTION(BlueprintCallable)
	//	static void SetLoadscreen(ULoadscreenWidget* nextLoadscreenBP);

	UFUNCTION(BlueprintCallable)
		static void LoadShellLVL();

	UFUNCTION(BlueprintCallable)
		static void AddEssentials();

	UFUNCTION(BlueprintCallable)
		static void FreeImporterResources();

	UFUNCTION(BlueprintCallable)
		static void GrabAddons();

	UFUNCTION(BlueprintCallable)
		static UTexture2D* GetLoadscreenTexture();

	UFUNCTION(BlueprintCallable)
		static UTexture2D* ImportTexture(const FString& textureName);

	UFUNCTION(BlueprintCallable)
		static USoundWave* ImportSound(const FString& soundName);

	UFUNCTION(BlueprintCallable)
		static FString GetLocalizedString(const FString& localizePath);

	UFUNCTION(BlueprintCallable)
		static bool ExecuteLUA(const FString& scriptName);

	UFUNCTION(BlueprintCallable)
		static void SwitchToMap(FName UELevel, FString levelScriptName, float loadscreenMinDisplayTime);

	UFUNCTION(BlueprintCallable)
		static void SwitchToMainMenu(float loadscreenMinDisplayTime);

	UFUNCTION(BlueprintCallable)
		static float GetSwitchProgress();

	UFUNCTION(BlueprintCallable)
		static ULevel* GetCurrentLevel();

	UFUNCTION(BlueprintCallable)
		static void AddDownloadableContent(FString addonName, FString scriptName);

	UFUNCTION(BlueprintCallable)
		static void AddToMapRotation(const FString& mapScript);

	UFUNCTION(BlueprintCallable)
		static void ClearMapRotation();

	UFUNCTION(BlueprintCallable)
		static void SwitchToNextMap();
};

