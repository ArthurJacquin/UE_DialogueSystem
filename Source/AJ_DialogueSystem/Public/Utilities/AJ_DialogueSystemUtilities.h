#pragma once

#include "CoreMinimal.h"
#include "DataAssets/AJ_Dialogue.h"
#include "Delegates/DelegateCombinations.h"
#include "Kismet/BlueprintAsyncActionBase.h"
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
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", Latent, LatentInfo = "LatentInfo"), Category = "AJ_DialogueSystem")
	static void PlayDialogue(UObject* WorldContext, FLatentActionInfo LatentInfo, UAJ_Dialogue* Dialogue);
};

/**
 * Latent action for waiting until the end of the dialogue
 */
class FAJ_WaitForDialogueCompletion : public FPendingLatentAction
{
public:
	FAJ_WaitForDialogueCompletion(FLatentActionInfo InLatentInfo, UAJ_DialogueWidget* InDialogueWidget)
		:LatentActionInfo(InLatentInfo), DialogueWidget(InDialogueWidget)
	{}

	/**
	 * Called every frame
	 */
	virtual void UpdateOperation(FLatentResponse& Response) override;

#if WITH_EDITOR
	virtual FString GetDescription() const override;
#endif

	/**
	 * Latent node info
	 */
	FLatentActionInfo LatentActionInfo;

	/**
	 * The widget for the running dialogue
	 */
	UAJ_DialogueWidget* DialogueWidget;
};