#pragma once

#include "CoreMinimal.h"
#include "DataAssets/AJ_Dialogue.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LatentActions.h"
#include "Widgets/AJ_DialogueWidget.h"

#include "AJ_DialogueSystemUtilities.generated.h"

/**
 * Utility functions to manage dialogues
 */
UCLASS(Abstract, ClassGroup = "AJ_DialogueSystem")
class AJ_DIALOGUESYSTEM_API UAJ_DialogueSystemUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "AJ_DialogueSystem")
	static UAJ_DialogueWidget* PlayDialogue(UObject* WorldContextObject, UAJ_Dialogue* Dialogue);
};
