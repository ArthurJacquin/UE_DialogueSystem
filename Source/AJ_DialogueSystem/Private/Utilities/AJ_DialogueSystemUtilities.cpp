#include "Utilities/AJ_DialogueSystemUtilities.h"

void UAJ_DialogueSystemUtilities::PlayDialogue(UObject* WorldContext, FLatentActionInfo LatentInfo, UAJ_Dialogue* Dialogue)
{
	UClass* WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/AJ_DialogueSystem/Widgets/WBP_Dialogue_Main.WBP_Dialogue_Main_C'"));
	if (WidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to play dialogue. Failed to load widget class."));
		return;
	}

	if (UAJ_DialogueWidget* Widget = CreateWidget<UAJ_DialogueWidget>(WorldContext->GetWorld(), WidgetClass))
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
