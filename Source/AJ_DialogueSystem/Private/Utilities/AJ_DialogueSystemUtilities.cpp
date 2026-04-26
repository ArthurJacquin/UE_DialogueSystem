#include "Utilities/AJ_DialogueSystemUtilities.h"
#include "Widgets/AJ_DialogueWidget.h"

void UAJ_DialogueSystemUtilities::PlayDialogue(UObject* WorldContextObject, UAJ_Dialogue* Dialogue)
{
	UClass* WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/AJ_DialogueSystem/Widgets/WBP_Dialogue_Main.WBP_Dialogue_Main_C'"));
	if (WidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to play dialogue. Failed to load widget class."));
		return;
	}

	if (UAJ_DialogueWidget* Widget = CreateWidget<UAJ_DialogueWidget>(WorldContextObject->GetWorld(), WidgetClass))
	{
		Widget->AddToViewport();
		Widget->PlayDialogue(Dialogue);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to play dialogue. Widget creation failed."));
	}
}
