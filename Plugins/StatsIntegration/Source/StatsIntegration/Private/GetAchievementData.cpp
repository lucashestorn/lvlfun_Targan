// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#include "GetAchievementData.h"
#include "Misc/DateTime.h"

static UTexture2D* ConvertToTexture2D(BYTE* data, int width, int height)
{
	UTexture2D* icon = UTexture2D::CreateTransient(width, height, PF_R8G8B8A8);

	uint8* MipData = (uint8*)icon->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(MipData, (void*)data, width * height * 4);

	icon->PlatformData->Mips[0].BulkData.Unlock();
	icon->NeverStream = true;
	icon->UpdateResource();

	return icon;
}

UTexture2D* UGetAchievementData::GetAchievementIcon(FString AchievementAPIName)
{
	if (SteamAPI_Init())
	{
		int iconIndex = SteamUserStats()->GetAchievementIcon(TCHAR_TO_ANSI(*AchievementAPIName));

		if (iconIndex != 0)
		{
			uint32 achievementIconWidth, achievementIconHeight;
			if (!SteamUtils()->GetImageSize(iconIndex, &achievementIconWidth, &achievementIconHeight))
			{
				return nullptr;
			}

			const int iconSizeinBytes = achievementIconWidth * achievementIconHeight * 4;
			BYTE* iconBuffer = new BYTE[iconSizeinBytes];

			if (!SteamUtils()->GetImageRGBA(iconIndex, (uint8*)iconBuffer, iconSizeinBytes))
			{
				return nullptr;
			}

			return ConvertToTexture2D(iconBuffer, achievementIconWidth, achievementIconHeight);
		}
	}
		
	return nullptr;
}

void UGetAchievementData::GetAchievementUnlockStatusAndUnlockTime(FString AchievementAPIName, bool& AchievementUnlocked, FDateTime& UnlockTime)
{
	bool achievementUnlockStatus;
	uint32 achievementUnlockTime;

	if (SteamUserStats()->GetAchievementAndUnlockTime(TCHAR_TO_ANSI(*AchievementAPIName), &achievementUnlockStatus, &achievementUnlockTime))
	{
		AchievementUnlocked = achievementUnlockStatus;
		UnlockTime = FDateTime::FromUnixTimestamp(achievementUnlockTime);
	}
}

float UGetAchievementData::GetGlobalAchievementAchievedPercent(FString AchievementAPIName)
{
	float achievementAchievedPercent;

	if (SteamUserStats()->GetAchievementAchievedPercent(TCHAR_TO_ANSI(*AchievementAPIName), &achievementAchievedPercent))
	{
		return (float)achievementAchievedPercent;
	}

	return 0.f;
}

FString UGetAchievementData::GetAchievementDisplayAttribute(FString AchievementAPIName, EAchievementDisplayAttributeUE4 Attribute)
{
	const char* displayAttributeValue;

	switch (Attribute)
	{
		case EAchievementDisplayAttributeUE4::ADA_EAttributeName:
			displayAttributeValue = SteamUserStats()->GetAchievementDisplayAttribute(TCHAR_TO_ANSI(*AchievementAPIName), "name");
			break;
		case EAchievementDisplayAttributeUE4::ADA_EAttributeDescription:
			displayAttributeValue = SteamUserStats()->GetAchievementDisplayAttribute(TCHAR_TO_ANSI(*AchievementAPIName), "desc");
			break;
		case EAchievementDisplayAttributeUE4::ADA_EAttributeHidden:
			displayAttributeValue = SteamUserStats()->GetAchievementDisplayAttribute(TCHAR_TO_ANSI(*AchievementAPIName), "hidden");
			break;
		default: 
			displayAttributeValue = "";
	}

	return FString(displayAttributeValue);
}

int32 UGetAchievementData::GetNumAchievements()
{
	if (SteamAPI_Init())
	{
		return SteamUserStats()->GetNumAchievements();
	}
	else
	{
		return 0;
	}
}

FString UGetAchievementData::GetAchievementAPIName(int32 AchievementIndex)
{
	if (SteamAPI_Init())
	{
		return FString(SteamUserStats()->GetAchievementName(AchievementIndex));
	}
	else
	{
		return "";
	}
}