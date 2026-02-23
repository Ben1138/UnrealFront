// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. See LICENSE in the root of this repository or https://www.gnu.org/licenses/ #include "UUnrealFrontGame.h"
#include "UnrealFrontGame.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealFront/UnrealFront.h"
#include "LuaAPI.h"


UUnrealFrontGame* UUnrealFrontGame::GameInstance = nullptr;

LibSWBF2::FNVHash UUnrealFrontGame::FNVHash(const FString& str)
{
	constexpr uint32_t FNV_prime = 16777619;
	constexpr uint32_t offset_basis = 2166136261;

	uint32_t hash = offset_basis;

	const char* buffer = TCHAR_TO_ANSI(*str);
	for (size_t i = 0; i < str.Len(); ++i)
	{
		char c = buffer[i];
		c |= 0x20;

		hash ^= c;
		hash *= FNV_prime;
	}

	return hash;
}

UUnrealFrontGame::UUnrealFrontGame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameInstance = this;
	bIsSwitchingMaps = false;
	bUELevelLoaded = false;
	bLVLWorldsImported = false;
	bMapSwitchFinishedEventFired = false;
}

void UUnrealFrontGame::Init()
{
	if (GameDataPath.IsEmpty())
	{
		UE_LOG(LogUF, Error, TEXT("No SWBF2 GameData path given!'"));
		return;
	}
	if (!VerifyGameDataPath())
	{
		UE_LOG(LogUF, Error, TEXT("Not a valid SWBF2 GameData path: '%s'!'"), *GameDataPath);
		return;
	}

	Importer = NewObject<UImporter>();
	const FString geo = GameDataPath + "/data/_lvl_pc/geo/geo1.lvl";
	const LibSWBF2::Handle hLevel = Importer->AddLevel(geo, nullptr);

	UE_LOG(LogUF, Warning, TEXT("Start Loading"));
	Importer->StartLoading();
	while (!Importer->IsDone())
	{
		//UE_LOG(LogUF, Warning, TEXT("Progress: %.0f"), Importer->GetOverallProgress() * 100.0f);
	}

	const LibSWBF2::Level* level = Importer->GetLevel(hLevel);
	//const LibSWBF2::Model* model1 = level->GetModel("geo_bldg_bunker_1");
	const LibSWBF2::Model* model2 = level->GetModel("geo_bldg_bunker_1_outside");

	FModelImportSettings modelSettings;
	FMaterialImportSettings materialSettings;
	//Importer->ImportModel(*model1, modelSettings, materialSettings, GetCurrentLevel());
	Importer->ImportModel(*model2, modelSettings, materialSettings, GetCurrentLevel());
}

void UUnrealFrontGame::Shutdown()
{
	Importer = nullptr;
}

UUnrealFrontGame* UUnrealFrontGame::GetGameInstance()
{
	return GameInstance;
}

bool UUnrealFrontGame::VerifyGameDataPath()
{
	FString hmk = FPaths::FindCorrectCase(GameDataPath);
	return FPaths::DirectoryExists(hmk);
	//return
	//	FPaths::DirectoryExists(GameDataPath) &&
	//	FPaths::FileExists(GameDataPath + "/BattlefrontII.exe") &&
	//	FPaths::DirectoryExists(GameDataPath + "/data/_lvl_pc") &&
	//	FPaths::FileExists(GameDataPath + "/data/_lvl_pc/shell.lvl") &&
	//	FPaths::FileExists(GameDataPath + "/data/_lvl_pc/mission.lvl");
}

UWorld* UUnrealFrontGame::GetCurrentWorld()
{
	if (GameInstance == nullptr)
		return nullptr;

	return GameInstance->GetWorld();
}

bool UUnrealFrontGame::IsEditor()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

//void UUnrealFrontGame::SetLoadscreen(ULoadscreenWidget* nextLoadscreenBP)
//{
//	GameInstance->LoadscreenBP = nextLoadscreenBP;
//}

bool UUnrealFrontGame::LVLExists(const FString& lvlPath)
{
	int32 index;
	if (!lvlPath.FindChar(';', index))
	{
		return FPaths::FileExists(lvlPath);
	}
	return FPaths::FileExists(lvlPath.Left(index));
}

LibSWBF2::Handle UUnrealFrontGame::AddLVL(FString lvlPath, const TSet<FString>* subLVLFilter, bool bEssential)
{
	bool bDlcLVL = lvlPath.ToLower().StartsWith("dc:");
	if (bDlcLVL)
	{
		lvlPath = lvlPath.RightChop(3);
	}

	FString lvlDLCPath = GameInstance->GameDataPath + "\\" + GameInstance->DLCEnvironmentPath + "\\data\\_LVL_PC\\" + lvlPath;
	FString lvlGlobalPath = GameInstance->GameDataPath + "\\data\\_LVL_PC\\" + lvlPath;

	if (bDlcLVL)
	{
		if (GameInstance->DLCEnvironmentPath.IsEmpty())
		{
			UE_LOG(LogUF, Error, TEXT("Cannot load DLC lvl '%s' with no DLCEnvironmentPath set!"), *lvlPath);
			return SWBF2HANDLE_INVALID;
		}
		lvlPath = lvlDLCPath;
	}
	else
	{
		if (bEssential && LVLExists(lvlDLCPath))
		{
			lvlPath = lvlDLCPath;
		}
		else
		{
			lvlPath = lvlGlobalPath;
		}
	}

	if (!LVLExists(lvlPath))
	{
		UE_LOG(LogUF, Error, TEXT("LVL '%s' does not exist!"), *lvlPath);
		return SWBF2HANDLE_INVALID;
	}

	return GameInstance->Importer->AddLevel(lvlPath, subLVLFilter);
}

LibSWBF2::Handle UUnrealFrontGame::AddSoundBank(FString soundBankPath)
{
	soundBankPath = GameInstance->GameDataPath + "\\data\\_LVL_PC\\" + soundBankPath;
	if (!FPaths::FileExists(soundBankPath))
	{
		UE_LOG(LogUF, Error, TEXT("Sound Bank '%s' does not exist!"), *soundBankPath);
		return SWBF2HANDLE_INVALID;
	}
	return GameInstance->Importer->AddSoundBank(soundBankPath);
}

void UUnrealFrontGame::LoadShellLVL()
{
	AddLVL("shell.lvl");
	AddLVL("common.lvl");
	GameInstance->Importer->StartLoading();
	GameInstance->bLoadingShellLVL = true;
}

void UUnrealFrontGame::AddEssentials()
{
	AddLVL("core.lvl", nullptr, true);
	AddLVL("shell.lvl", nullptr, true);
	AddLVL("common.lvl", nullptr, true);
	AddLVL("mission.lvl", nullptr, true);
	AddSoundBank("sound\\common.bnk");
}

void UUnrealFrontGame::FreeImporterResources()
{
	GameInstance->Importer->FreeAll();
}

void UUnrealFrontGame::GrabAddons()
{
	GameInstance->DownloadableContents.Empty();

	FString addonsPath = GameInstance->GameDataPath + "\\addon";
	if (FPaths::DirectoryExists(addonsPath))
	{
		IFileManager& fileMan = IFileManager::Get();
		TArray<FString> found;
		fileMan.FindFilesRecursive(found, *addonsPath, TEXT("addme.script"), true, false);

		for (FString addonScript : found)
		{
			addonScript = addonScript.Replace(TEXT("/"), TEXT("\\"));
			GameInstance->AddmeScriptHandles.Add(GameInstance->Importer->AddLevel(addonScript));
			UE_LOG(LogUF, Display, TEXT("Found addon script: %s"), *addonScript);
		}

		found.Empty();
		fileMan.FindFilesRecursive(found, *addonsPath, TEXT("mission.lvl"), true, false);
		for (FString missionLVL : found)
		{
			missionLVL = missionLVL.Replace(TEXT("/"), TEXT("\\"));
			GameInstance->Importer->AddLevel(missionLVL);
			UE_LOG(LogUF, Display, TEXT("Found mission.lvl: %s"), *missionLVL);
		}
	}
}

void UUnrealFrontGame::Tick(float DeltaTime)
{
	if (bLoadingShellLVL && Importer->IsDone())
	{
		bLoadingShellLVL = false;
		OnShellLVLFinished.Broadcast();
	}
	else if (bIsSwitchingMaps)
	{
		if (bLoadscreenFadeInFinished && !bLoadscreenLoadedEventFired && (LoadscreenLVL != SWBF2HANDLE_INVALID) && Importer->GetLevelStatus(LoadscreenLVL) == LibSWBF2::ELoadStatus::Loaded)
		{
			bLoadscreenLoadedEventFired = true;
			OnLoadTextureFinished.Broadcast();
		}

		// grab loadscreen LVL for custom maps
		if (!DLCEnvironmentPath.IsEmpty() && !bCustomLoadscreenLoaded)
		{
			TSet<LibSWBF2::Handle> levelHandles = Importer->GetLoadedLevels();
			for (LibSWBF2::Handle handle : levelHandles)
			{
				if (Importer->GetLevelStatus(handle) == LibSWBF2::ELoadStatus::Loaded)
				{
					LibSWBF2::Level* level = Importer->GetLevel(handle);
					if (level == nullptr)
					{
						UE_LOG(LogUF, Error, TEXT("Level is loaded, but couldn't grab Level instance! This should not happen!"));
						continue;
					}
					FString levelPath = LIBSTR_TO_TCHAR(level->GetLevelPath());
					if (levelPath.Contains(DLCEnvironmentPath) && levelPath.Contains("load") && level->GetTexture("loadscreen") != nullptr)
					{
						LoadscreenLVL = handle;
						bLoadscreenLoadedEventFired = false;
						bCustomLoadscreenLoaded = true;
						break;
					}
				}
			}
		}

		for (LibSWBF2::Handle scriptHandle : AddmeScriptHandles)
		{
			if (Importer->GetLevelStatus(scriptHandle) == LibSWBF2::ELoadStatus::Loaded)
			{
				// addme scripts call "ReadDataFile" to load their respective Core.lvl
				Importer->ExecuteLUA(scriptHandle, "addme");
				Importer->StartLoading();
				AddmeScriptHandles.Remove(scriptHandle);
				
				// do not further iterate altered set
				return;
			}
		}

		bool bAllLVLsLoaded = Importer->IsDone();
		if (!bLVLWorldsImported && bAllLVLsLoaded && bUELevelLoaded)
		{
			//Importer->ImportTo(GetCurrentLevel(), MaterialSettings);
			bLVLWorldsImported = true;
		}

		if (!bMapSwitchFinishedEventFired && bAllLVLsLoaded && bUELevelLoaded && bLoadscreenLoadedEventFired)
		{
			UWorld* world = UUnrealFrontGame::GetCurrentWorld();
			float duration = UGameplayStatics::GetRealTimeSeconds(world) - MapSwitchStartedTime;
			OnMapSwitchFinished.Broadcast(duration);
			bMapSwitchFinishedEventFired = true;

			//if (CurrentLoadscreen->BlendOutAnimation == nullptr)
			//{
			//	UE_LOG(LogUF, Warning, TEXT("Given loadscreenBP has no BlendOut animation set!"));
			//}
			//else
			//{
			//	FTimerHandle handle;
			//	FTimerDelegate callback;
			//	callback.BindLambda([this]()
			//	{
			//		UE_LOG(LogUF, Display, TEXT("Fade out loadscreen..."));
			//		UUMGSequencePlayer* player = CurrentLoadscreen->PlayAnimation(CurrentLoadscreen->BlendOutAnimation);
			//		player->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer& player)
			//		{
			//			UE_LOG(LogUF, Display, TEXT("Loadscreen removed!"));
			//			CurrentLoadscreen->RemoveFromParent();
			//		});
			//	});
			//	GetTimerManager().SetTimer(
			//		handle, 
			//		callback,
			//		FMath::Max(LoadscreenMinDisplayTime - duration, 0.1f), 
			//		false
			//	);
			//}

			if (CurrentUnrealLevel != "MainMenu")
			{
				ULuaAPI::Call_ScriptPostLoad();
			}

			bIsSwitchingMaps = false;
			UE_LOG(LogUF, Display, TEXT("Map switch finished!"));
		}
	}
}

void UUnrealFrontGame::OnUELevelLoaded()
{
	bUELevelLoaded = true;
	UE_LOG(LogUF, Display, TEXT("Unreal Level loaded"));
}

bool UUnrealFrontGame::IsTickable() const
{
	return GIsRunning && (bIsSwitchingMaps || bLoadingShellLVL);
}

TStatId UUnrealFrontGame::GetStatId() const
{
	return UObject::GetStatID();
}

UTexture2D* UUnrealFrontGame::GetLoadscreenTexture()
{
	if (!GameInstance->bIsSwitchingMaps || GameInstance->LoadscreenLVL == SWBF2HANDLE_INVALID || GameInstance->Importer->GetLevelStatus(GameInstance->LoadscreenLVL) != LibSWBF2::ELoadStatus::Loaded)
	{
		UE_LOG(LogUF, Warning, TEXT("Tried to call GetLoadscreenTexture() before loading the texture is finished!"));
		return nullptr;
	}

	LibSWBF2::Level* loadscreen = GameInstance->Importer->GetLevel(GameInstance->LoadscreenLVL);
	const LibSWBF2::List<LibSWBF2::Texture>& textures = loadscreen->GetTextures();
	return GameInstance->Importer->ImportTexture(textures[FMath::RandRange(0, textures.Size() - 1)]);
}

UTexture2D* UUnrealFrontGame::ImportTexture(const FString& textureName)
{
	if (GameInstance == nullptr || GameInstance->Importer == nullptr) return nullptr;
	return GameInstance->Importer->ImportTexture(textureName);
}

USoundWave* UUnrealFrontGame::ImportSound(const FString& soundName)
{
	if (GameInstance == nullptr || GameInstance->Importer == nullptr) return nullptr;
	return GameInstance->Importer->ImportSound(soundName);
}

bool UUnrealFrontGame::ExecuteLUA(const FString& scriptName)
{
	return GameInstance->Importer->ExecuteLUA(scriptName);
}

void UUnrealFrontGame::SwitchToMap(FName UELevel, FString levelScriptName, float loadscreenMinDisplayTime)
{
	if (UELevel.IsNone())
	{
		UE_LOG(LogUF, Error, TEXT("Given UELevel is EMPTY!"));
		return;
	}
	if (levelScriptName.IsEmpty())
	{
		UE_LOG(LogUF, Error, TEXT("Given levelScriptName is EMPTY!"));
		return;
	}
	//if (GameInstance->LoadscreenBP == nullptr)
	//{
	//	UE_LOG(LogUF, Error, TEXT("LoadscreenBP is NULL!"));
	//	return;
	//}

	UE_LOG(LogUF, Display, TEXT("Start Map Switch to: %s, %s"), *UELevel.ToString(), *levelScriptName);

	GameInstance->bUELevelLoaded = false;
	GameInstance->bLVLWorldsImported = false;
	GameInstance->bLoadscreenLoadedEventFired = false;
	GameInstance->bMapSwitchFinishedEventFired = false;
	GameInstance->bLoadscreenFadeInFinished = false;
	GameInstance->bCustomLoadscreenLoaded = false;
	//GameInstance->CurrentLoadscreen = GameInstance->LoadscreenBP;
	GameInstance->AddmeScriptHandles.Empty();

	GameInstance->bIsSwitchingMaps = true;
	GameInstance->OnMapSwitch.Broadcast();

	// clear LUA vm
	ULuaAPI::CloseEnvironment();
	ULuaAPI::OpenEnvironment();

	// Check if script to execute is part of a DLC and switch environment accordingly
	FString* addonName = GameInstance->DownloadableContents.Find(levelScriptName);
	FString previousDLVEnv = GameInstance->DLCEnvironmentPath;
	if (addonName != nullptr)
	{
		GameInstance->DLCEnvironmentPath = "addon\\" + *addonName;
	}
	else
	{
		GameInstance->DLCEnvironmentPath = "";
	}

	if (!GameInstance->ExecuteLUA(levelScriptName))
	{
		UE_LOG(LogUF, Error, TEXT("Map switch failed! Script '%s' not found!"), *levelScriptName);
		GameInstance->bIsSwitchingMaps = false;
		return;
	}

	UWorld* world = UUnrealFrontGame::GetCurrentWorld();
	GameInstance->MapSwitchStartedTime = UGameplayStatics::GetRealTimeSeconds(world);

	//GameInstance->LoadscreenMinDisplayTime = loadscreenMinDisplayTime;
	//GameInstance->CurrentLoadscreen->SetAnimations();
	//GameInstance->CurrentLoadscreen->AddToViewport(1000); // loadscreen above (hopefully) everything else
	//if (GameInstance->CurrentLoadscreen->BlendInAnimation == nullptr)
	//{
	//	UE_LOG(LogUF, Warning, TEXT("Given loadscreenBP has no BlendIn animation set!"));
	//}
	//else
	//{
	//	UUMGSequencePlayer* player = GameInstance->CurrentLoadscreen->PlayAnimation(GameInstance->CurrentLoadscreen->BlendInAnimation);
	//	 start loading next UELevel and unloading current UE level AFTER loadscreen is fade in
	//	player->OnSequenceFinishedPlaying().AddLambda([world, UELevel](UUMGSequencePlayer& player)
	//	{
	//		FLatentActionInfo info;
	//		info.CallbackTarget = GameInstance;
	//		info.ExecutionFunction = "OnUELevelLoaded"; 
	//		info.UUID = 1138;
	//		info.Linkage = 0;
	//		UGameplayStatics::LoadStreamLevel(world, UELevel, true, false, info);

	//		if (!GameInstance->CurrentUnrealLevel.IsNone())
	//		{
	//			UGameplayStatics::UnloadStreamLevel(world, GameInstance->CurrentUnrealLevel, FLatentActionInfo(), false);
	//		}
	//		GameInstance->CurrentUnrealLevel = UELevel;
	//		GameInstance->bLoadscreenFadeInFinished = true;
	//	});
	//}

	// another special case... apparently load screen tetxures for shipped maps are not
	// specified / referenced anywhere, instead it seems like the game itself determines 
	// which load screen to use when loading a map lvl. Additionally, there are multiple 
	// textures available for each map. Load screen textures are stored in their lvl counterparts,
	// e.g. load/geo1.lvl. Unlike the world name, the load screen textures contain
	// an underscore in their names and seem to be chosen randomly on loadup.
	// For geo1 for example in load/geo1.lvl there are:
	// - geo_1
	// - geo_2
	// - geo_3
	// - geo_4

	// TODO: find a better solution for this.. maybe an explicit script->loadscreen map?
	FString loadscreenPath = "load\\" + levelScriptName.Left(4) + ".lvl";
	FString loadscreenGlobalPath = GameInstance->GameDataPath + "\\data\\_lvl_pc\\" + loadscreenPath;
	if (!FPaths::FileExists(loadscreenGlobalPath))
	{
		if (UELevel == "MainMenu")
		{
			loadscreenPath = "load\\gal_con.lvl";
		}
		else
		{
			loadscreenPath = "load\\common.lvl";
		}
	}

	GameInstance->FreeImporterResources();
	GameInstance->LoadscreenLVL = AddLVL(loadscreenPath);
	GameInstance->AddEssentials();

	if (UELevel == "MainMenu")
	{
		GrabAddons();
	}
	else
	{
		// Execute addon.script files AFTER missionlist has been executed!
		ULuaAPI::Call_ScriptInit();
	}



	GameInstance->Importer->StartLoading();
}

void UUnrealFrontGame::SwitchToMainMenu(float loadscreenMinDisplayTime)
{
	SwitchToMap("MainMenu", "missionlist", loadscreenMinDisplayTime);
}

ULevel* UUnrealFrontGame::GetCurrentLevel()
{
	if (GameInstance == nullptr)
		return nullptr;

	const TArray<ULevel*>& levels = GameInstance->GetWorld()->GetLevels();
	for (ULevel* level : levels)
	{
		FString name;
		level->GetPathName(nullptr, name);
		//if (level->GetFName() == GameInstance->CurrentUnrealLevel)
		if (name.Contains(GameInstance->CurrentUnrealLevel.ToString()))
		{
			return level;
		}
	}
	return nullptr;
}

void UUnrealFrontGame::AddDownloadableContent(FString addonName, FString scriptName)
{
	GameInstance->DownloadableContents.Add(scriptName, addonName);
}

FString UUnrealFrontGame::GetLocalizedString(const FString& localizePath)
{
	FString localized;
	GameInstance->Importer->GetLocalizedString("english", localizePath, localized);
	return localized;
}

void UUnrealFrontGame::AddToMapRotation(const FString& mapScript)
{
	GameInstance->MapRotation.Add(mapScript);
}

void UUnrealFrontGame::ClearMapRotation()
{
	GameInstance->MapRotation.Empty();
	GameInstance->CurrentMapRotationIndex = -1;
}

void UUnrealFrontGame::SwitchToNextMap()
{
	if (GameInstance->MapRotation.Num() == 0)
	{
		UE_LOG(LogUF, Warning, TEXT("Cannot switch to next map in rotation, no maps present!"));
		return;
	}

	if (++GameInstance->CurrentMapRotationIndex >= GameInstance->MapRotation.Num())
	{
		GameInstance->CurrentMapRotationIndex = 0;
	}

	SwitchToMap(
		"MapLevel", 
		GameInstance->MapRotation[GameInstance->CurrentMapRotationIndex], 
		1.0f
	);
}

float UUnrealFrontGame::GetSwitchProgress()
{
	return GameInstance->Importer->GetOverallProgress();
}
