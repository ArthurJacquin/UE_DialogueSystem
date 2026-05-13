#include "Utilities/AJ_DialogueSystemUtilities.h"

#include "Settings/AJ_DialogueSystemSettings.h"

void UAJ_DialogueSystemUtilities::PlayDialogue(UObject* WorldContext, FLatentActionInfo LatentInfo, UAJ_Dialogue* Dialogue)
{
	const UAJ_DialogueSystemSettings* const Settings = GetDefault<UAJ_DialogueSystemSettings>();
	if (!IsValid(Settings) || Settings->DialogueWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to play dialogue. Failed to load widget class."));
		return;
	}

	if (UAJ_DialogueWidget* Widget = CreateWidget<UAJ_DialogueWidget>(WorldContext->GetWorld(), Settings->DialogueWidgetClass))
	{
		Widget->AddToViewport();
		Widget->PlayDialogue(Dialogue);

		UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
		if (World == nullptr)
		{
			return;
		}

		// On Complete latent action
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		FAJ_WaitForDialogueCompletion* LatentAction = new FAJ_WaitForDialogueCompletion(LatentInfo, Widget);
		LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, LatentAction);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to play dialogue. Widget creation failed."));
	}
}

void FAJ_WaitForDialogueCompletion::UpdateOperation(FLatentResponse& Response)
{
	if (DialogueWidget->bIsDialogueCompleted)
	{
		DialogueWidget->RemoveFromParent();
		Response.FinishAndTriggerIf(true, LatentActionInfo.ExecutionFunction, LatentActionInfo.Linkage, LatentActionInfo.CallbackTarget);
		return;
	}
}

FString FAJ_WaitForDialogueCompletion::GetDescription() const
{
	return TEXT("Waiting for dialogue to complete...");
}
