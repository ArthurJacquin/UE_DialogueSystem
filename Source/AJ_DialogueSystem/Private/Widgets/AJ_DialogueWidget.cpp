#include "Widgets/AJ_DialogueWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataAssets/AJ_Dialogue.h"

void UAJ_DialogueWidget::PlayDialogue(UAJ_Dialogue* InDialogue)
{
	Dialogue = InDialogue;
	CurrentEntryId = 0;

	if (Dialogue->DialogueEntries.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("No entry found for dialogue. Removing widget."));
		ExitDialogue(false);
		return;
	}

	SetupDialogueEntry(Dialogue->DialogueEntries[0]);
}

void UAJ_DialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ContinueButton->OnClicked.AddDynamic(this, &UAJ_DialogueWidget::ContinueDialogue);
}

void UAJ_DialogueWidget::ContinueDialogue()
{
	++CurrentEntryId;

	if (CurrentEntryId >= Dialogue->DialogueEntries.Num())
	{
		ExitDialogue(true);
		return;
	}

	SetupDialogueEntry(Dialogue->DialogueEntries[CurrentEntryId]);
}

void UAJ_DialogueWidget::ExitDialogue(bool bCallEvent)
{
	if (bCallEvent)
	{
		//TODO: call event OnDialogueCompleted
	}

	RemoveFromParent();
}

void UAJ_DialogueWidget::SetupDialogueEntry(const FAJ_DialogueEntry& Entry)
{
	const UAJ_DialogueSpeakerData* const SpeakerData = Entry.SpeakerData;
	if (SpeakerData->BaseImage != nullptr)
	{
		Speaker1Image->SetBrushFromSoftTexture(SpeakerData->BaseImage, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No image set for character %s."), *SpeakerData->Name.ToString());
	}
	Speaker1NameText->SetText(SpeakerData->Name);
	Speaker1NameText->SetColorAndOpacity(SpeakerData->Color);

	ScriptLineText->SetText(Entry.ScriptLine);

	//TODO: Handle multiple speakers
	Speaker2NameText->SetVisibility(ESlateVisibility::Hidden);
	Speaker2Image->SetVisibility(ESlateVisibility::Hidden);
}
