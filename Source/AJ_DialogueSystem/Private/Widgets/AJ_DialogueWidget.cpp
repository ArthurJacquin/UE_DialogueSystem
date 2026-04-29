#include "Widgets/AJ_DialogueWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataAssets/AJ_Dialogue.h"

DEFINE_LOG_CATEGORY(AJ_DialogueWidgetLog);

void UAJ_DialogueWidget::PlayDialogue(UAJ_Dialogue* const InDialogue)
{
	Dialogue = InDialogue;
	CurrentEntryId = 0;

	if (Dialogue->DialogueEntries.IsEmpty())
	{
		UE_LOG(AJ_DialogueWidgetLog, Error, TEXT("No entry found for dialogue. Removing widget."));
		ExitDialogue();
		return;
	}

	//Setup general data
	CacheDialogueData();
	SetupSpeakersWidgets();

	// Setup first dialogue entry
	SetupDialogueEntry(Dialogue->DialogueEntries[0]);
}

void UAJ_DialogueWidget::OnSpeakerStateChanged_Implementation(bool bIsSpeaking, const FAJ_SpeakerWidgets& InSpeakerWidgets, const UAJ_DialogueSpeakerData* SpeakerData)
{
	if (UTextBlock* NameText = InSpeakerWidgets.SpeakerNameText)
	{
		NameText->SetVisibility(bIsSpeaking ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (UImage* Image = InSpeakerWidgets.SpeakerImage)
	{
		Image->SetVisibility(bIsSpeaking ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UAJ_DialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ContinueButton->OnClicked.AddDynamic(this, &UAJ_DialogueWidget::ContinueDialogue);
}

void UAJ_DialogueWidget::CacheDialogueData()
{
	// Cache speakers list
	for (const FAJ_DialogueEntry& Entry : Dialogue->DialogueEntries)
	{
		Speakers.AddUnique(Entry.SpeakerData);

		if (Speakers.Num() == 2)
		{
			break;
		}
	}
}

void UAJ_DialogueWidget::SetupSpeakersWidgets()
{
	if (!GetClass()->IsFunctionImplementedInScript(TEXT("AssignSpeakersWidgets")))
	{
		UE_LOG(AJ_DialogueWidgetLog, Error, TEXT("AssignSpeakersWidgets not implemented in BP: %s"), *GetName());
		return;
	}
	else
	{
		AssignSpeakersWidgets();
	}

	for (int32 i = 0; i < Speakers.Num(); ++i)
	{
		const UAJ_DialogueSpeakerData* const Speaker = Speakers[i];
		if (UTextBlock* SpeakerNameText = SpeakerWidgets[i].SpeakerNameText)
		{
			SpeakerNameText->SetText(Speaker->Name);
			SpeakerNameText->SetColorAndOpacity(Speaker->Color);
		}

		if (UImage* SpeakerImage = SpeakerWidgets[i].SpeakerImage)
		{
			SpeakerImage->SetBrushFromSoftTexture(Speaker->BaseImage);
		}
	}
}

void UAJ_DialogueWidget::ContinueDialogue()
{
	++CurrentEntryId;

	if (CurrentEntryId >= Dialogue->DialogueEntries.Num())
	{
		ExitDialogue();
		return;
	}

	SetupDialogueEntry(Dialogue->DialogueEntries[CurrentEntryId]);
}

void UAJ_DialogueWidget::ExitDialogue()
{
	bIsDialogueCompleted = true;
	OnDialogueCompleted.Broadcast();
}

void UAJ_DialogueWidget::SetupDialogueEntry(const FAJ_DialogueEntry& Entry)
{
	UAJ_DialogueSpeakerData* const SpeakerData = Entry.SpeakerData;
	int32 SpeakerId = GetSpeakerId(SpeakerData);

	SetSpeaker(SpeakerId);

	ScriptLineText->SetText(Entry.ScriptLine);
}

void UAJ_DialogueWidget::SetSpeaker(const int32& SpeakerId)
{
	for (int32 i = 0; i < SpeakerWidgets.Num(); ++i)
	{
		const FAJ_SpeakerWidgets& Widgets = SpeakerWidgets[i];

		// Disable widgets of speakers that are not in this dialogue entry
		if (i >= Speakers.Num())
		{
			Widgets.SpeakerNameText->SetVisibility(ESlateVisibility::Collapsed);
			Widgets.SpeakerImage->SetVisibility(ESlateVisibility::Collapsed);
			continue;
		}

		// Update speakers in this dialogue entry
		OnSpeakerStateChanged(i == SpeakerId, Widgets, Speakers[i]);
	}
}

int32 UAJ_DialogueWidget::GetSpeakerId(UAJ_DialogueSpeakerData* const InSpeakerData) const
{
	int32 Id = -1;
	Speakers.Find(InSpeakerData, Id);
	return Id;
}
