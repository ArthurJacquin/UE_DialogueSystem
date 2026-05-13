// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/AJ_DialogueSystemSettings.h"

FName UAJ_DialogueSystemSettings::GetCategoryName() const
{
	return TEXT("Plugins");
}

#if WITH_EDITOR
FText UAJ_DialogueSystemSettings::GetSectionText() const
{
	return NSLOCTEXT("AJ_DialogueSystemSettings", "AJ_DialogueSystemSettingsSection", "AJ Dialogue System");
}

FName UAJ_DialogueSystemSettings::GetSectionName() const
{
	return TEXT("AJ Dialogue System");
}
#endif
