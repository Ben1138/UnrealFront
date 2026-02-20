// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. See LICENSE in the root of this repository or https://www.gnu.org/licenses/
#pragma once
#include "CoreMinimal.h"
#include "Structs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <LibSWBF2/LibSWBF2.h>
#include <lua.hpp>

#include "LuaAPI.generated.h"


UCLASS(BlueprintType)
class ULuaAPI : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool ExecuteLUA(const LibSWBF2::Script& luaScript);
	
	UFUNCTION(BlueprintCallable)
		static bool ExecuteLUAFile(FString luaFilePath);
	
	UFUNCTION(BlueprintCallable)
		static void OpenEnvironment();
	
	UFUNCTION(BlueprintCallable)
		static void CloseEnvironment();

	UFUNCTION(BlueprintCallable)
		static TArray<FLuaMapEntry> GetSPMapList();

	UFUNCTION(BlueprintCallable)
		static TArray<FLuaMapEntry> GetMPMapList();

	UFUNCTION(BlueprintCallable)
		static void Call_ScriptInit();

	UFUNCTION(BlueprintCallable)
		static void Call_ScriptPostLoad();

	UFUNCTION(BlueprintCallable)
		static TArray<FLuaMapEra> GetEras();
	
	UFUNCTION(BlueprintCallable)
		static TArray<FLuaMapMode> GetModes();

	//UFUNCTION(BlueprintCallable)
	//	static void Call_missionlist_ExpandMaplist(bool bForMP);

	//UFUNCTION(BlueprintCallable)
	//	static void PrintExpandedMapList();

private:
	static bool CheckStatus(const FString& scriptName, int status);

	static TArray<FLuaKeyValuePair> IterateTable(lua_State* L, int index);
	static TArray<FLuaMapEntry> GetMapList(const FString& mapTableName);
	static inline FString GetTableKeyValueAsString(const FString& key);
	static inline bool GetTableKeyValueAsBool(const FString& key);

	/* SWBF2 LUA API implementations */
	static int ScriptCB_GetPlatform(lua_State* lua);
	static int ScriptCB_GetOnlineService(lua_State* lua);
	static int ScriptCB_GetLanguage(lua_State* lua);
	static int ScriptCB_GetCONMaxTimeLimit(lua_State* lua);	// game time limit as set in game options menu
	static int ScriptCB_GetCONNumBots(lua_State* lua);
	static int ScriptCB_SetNumBots(lua_State* lua);
	static int ScriptCB_IsMissionSetupSaved(lua_State* lua);
	static int ScriptCB_LoadMissionSetup(lua_State* lua);

	// localization, formatting and unicode
	static int ScriptCB_getlocalizestr(lua_State* lua);
	static int ScriptCB_tounicode(lua_State* lua);
	static int ScriptCB_ununicode(lua_State* lua);
	static int ScriptCB_usprintf(lua_State* lua);

	static int ScriptCB_DoFile(lua_State* lua);
	
	static int SetPS2ModelMemory(lua_State* lua);
	static int StealArtistHeap(lua_State* lua);
	static int SetTeamAggressiveness(lua_State* lua);
	static int SetMemoryPoolSize(lua_State* lua);
	static int ClearWalkers(lua_State* lua);
	static int AddWalkerType(lua_State* lua);
	static int SetSpawnDelay(lua_State* lua);
	static int SetHeroClass(lua_State* lua);
	static int SetTeamAsEnemy(lua_State* lua);
	static int SetTeamAsFriend(lua_State* lua);
	static int SetTeamName(lua_State* lua);
	static int SetUnitCount(lua_State* lua);
	static int AddUnitClass(lua_State* lua);
	static int SetDenseEnvironment(lua_State* lua);
	static int SetMinFlyHeight(lua_State* lua);
	static int SetMaxFlyHeight(lua_State* lua);
	static int SetMaxPlayerFlyHeight(lua_State* lua);
	static int SetAttackingTeam(lua_State* lua);
	static int AddCameraShot(lua_State* lua);
	static int SetTeamIcon(lua_State* lua);
	static int SetBleedRate(lua_State* lua);
	static int GetReinforcementCount(lua_State* lua);
	static int SetReinforcementCount(lua_State* lua);
	static int AddReinforcements(lua_State* lua);
	static int SetMapNorthAngle(lua_State* lua);
	static int AISnipeSuitabilityDist(lua_State* lua);
	static int EnableSPHeroRules(lua_State* lua);
	static int AddDeathRegion(lua_State* lua);
	static int SetProperty(lua_State* lua);
	static int DisableBarriers(lua_State* lua);
	static int PlayAnimation(lua_State* lua);
	static int SetUberMode(lua_State* lua);
	static int GetCommandPostBleedValue(lua_State* lua);

	// Sound
	static int OpenAudioStream(lua_State* lua);
	static int AudioStreamAppendSegments(lua_State* lua);
	static int SetBleedingVoiceOver(lua_State* lua);
	static int SetLowReinforcementsVoiceOver(lua_State* lua);
	static int SetOutOfBoundsVoiceOver(lua_State* lua);
	static int SetAmbientMusic(lua_State* lua);
	static int SetVictoryMusic(lua_State* lua);
	static int SetDefeatMusic(lua_State* lua);
	static int SetSoundEffect(lua_State* lua);

	// IMPORTANT
	static int ReadDataFile(lua_State* lua);
	static int AddDownloadableContent(lua_State* lua);
};

