#include "Widgets/AJ_DialogueWidget.h"

#include "Components/AudioComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Sound/AmbientSound.h"

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
	
	StateImage->SetBrushFromTexture(GoToNextEntryImage);

	// Setup first dialogue entry
	SetupDialogueEntry(Dialogue->DialogueEntries[0]);
}

void UAJ_DialogueWidget::PlayUISound(USoundWave* const Sound, float Pitch)
{
	if (!IsValid(AmbientSound))
	{
		UE_LOG(AJ_DialogueWidgetLog, Error, TEXT("Failed to play sound, AmbientSound actor not valid."));
		return;
	}
	
	AmbientSound->GetAudioComponent()->SetSound(Sound);
	AmbientSound->GetAudioComponent()->SetPitchMultiplier(Pitch);
	AmbientSound->GetAudioComponent()->Play();
}

void UAJ_DialogueWidget::StopUISound()
{
	if (!IsValid(AmbientSound))
	{
		UE_LOG(AJ_DialogueWidgetLog, Error, TEXT("Failed to stop sound, AmbientSound actor not valid."));
		return;
	}
	
	AmbientSound->GetAudioComponent()->Stop();
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
	
	if (UWorld* World = GetWorld())
	{
		AmbientSound = World->SpawnActor<AAmbientSound>();
	}
}

void UAJ_DialogueWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (bTextAnimInProgress)
	{
		UpdateTextAnimation(InDeltaTime);
	}
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
			if (!Speaker->ImagesPerEmotion.Contains(EAJ_DialogueEmotion::Neutral))
			{
				UE_LOG(AJ_DialogueWidgetLog, Error, TEXT("No neutral emotion found for speaker %s"), *Speaker->Name.ToString());
			}
			else
			{
				// Set default image to the neutral one
				if (Speaker->ImagesPerEmotion.Contains(EAJ_DialogueEmotion::Neutral))
				{
					UTexture* const DefaultImage = Speaker->ImagesPerEmotion[EAJ_DialogueEmotion::Neutral].DefaultImage.LoadSynchronous();
					SpeakerImage->GetDynamicMaterial()->SetTextureParameterValue(TEXT("DefaultTexture"), DefaultImage);
				}
			}
		}
	}
}

void UAJ_DialogueWidget::ContinueDialogue()
{
	// If we have an anim running, finish it
	if (bTextAnimInProgress)
	{
		StopAnimations();
	}
	else // Go to next entry
	{
		++CurrentEntryId;

		if (CurrentEntryId >= Dialogue->DialogueEntries.Num())
		{
			ExitDialogue();
			return;
		}

		SetupDialogueEntry(Dialogue->DialogueEntries[CurrentEntryId]);
	}
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
	
	// Toggle widgets for the speaker
	SetSpeaker(SpeakerId, Entry.Emotion);

	if (bUseAnimations)
	{
		// Init animation data
		TextAnimationData = FAJ_TextAnimationData();
		TextAnimationData.SpeakerId = SpeakerId;
		TextAnimationData.DialogueEntry = Entry;
		TextAnimationData.FinalScriptLine = Entry.ScriptLine.ToString();
		
		StartAnimations();
	}
	else
	{
		// Set speaker emotion default image
		const FAJ_SpeakerWidgets& Widgets = SpeakerWidgets[SpeakerId];
		UTexture* const Image = SpeakerData->ImagesPerEmotion[Entry.Emotion].DefaultImage.LoadSynchronous();
		Widgets.SpeakerImage->GetDynamicMaterial()->SetTextureParameterValue(TEXT("DefaultTexture"), Image);
		
		// Set script line
		ScriptLineText->SetText(Entry.ScriptLine);
	}
}

void UAJ_DialogueWidget::SetSpeaker(const int32& SpeakerId, const EAJ_DialogueEmotion& Emotion)
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

void UAJ_DialogueWidget::StartAnimations()
{
	const FAJ_SpeakerWidgets& Widgets = SpeakerWidgets[TextAnimationData.SpeakerId];
	const FAJ_DialogueEntry* const Entry = &TextAnimationData.DialogueEntry;
	if (bEnableAnimatedText)
	{
		ScriptLineText->SetText(FText());
		StateImage->SetBrushFromTexture(FastForwardImage);
		bTextAnimInProgress = true;
	}
	
	if (bEnableTalkingAnimations)
	{
		UMaterialInstanceDynamic* const ImageMaterial = Widgets.SpeakerImage->GetDynamicMaterial();
		UTexture* const Flipbook = Entry->SpeakerData->ImagesPerEmotion[Entry->Emotion].TalkingFlipbook.LoadSynchronous();
		ImageMaterial->SetTextureParameterValue(TEXT("Flipbook"), Flipbook);
		ImageMaterial->SetScalarParameterValue(TEXT("IsSpeaking"), true);
	}
}

void UAJ_DialogueWidget::StopAnimations()
{
	if (bEnableAnimatedText)
	{
		ScriptLineText->SetText(FText::FromString(TextAnimationData.FinalScriptLine));
		StateImage->SetBrushFromTexture(GoToNextEntryImage);
		bTextAnimInProgress = false;
	}
	
	if (bEnableTalkingAnimations)
	{
		const FAJ_SpeakerWidgets& Widgets = SpeakerWidgets[TextAnimationData.SpeakerId];
		UMaterialInstanceDynamic* const ImageMaterial = Widgets.SpeakerImage->GetDynamicMaterial();
		ImageMaterial->SetScalarParameterValue(TEXT("IsSpeaking"), false);
	}
	
	OnSpeakerFinishSpeach();
}

void UAJ_DialogueWidget::UpdateTextAnimation(const float& DeltaTime)
{
	TextAnimationData.CurrentTextAnimationTime += DeltaTime;
	if (TextAnimationData.CurrentTextAnimationTime > TimeBetweenCharacters)
	{
		++TextAnimationData.CurrentCharacterId;
		
		TextAnimationData.CurrentDisplayedText = FText::FromString(
			TextAnimationData.FinalScriptLine.LeftChop(TextAnimationData.FinalScriptLine.Len() - TextAnimationData.CurrentCharacterId)
			);
		ScriptLineText->SetText(TextAnimationData.CurrentDisplayedText);
		
		// BP event for audio or any other effect happening on each letter
		const FString LetterAdded = TextAnimationData.CurrentDisplayedText.ToString().RightChop(TextAnimationData.CurrentCharacterId - 1);
		OnScriptLineLetterAdded(LetterAdded, TextAnimationData.DialogueEntry.SpeakerData);
		
		if (TextAnimationData.CurrentCharacterId == TextAnimationData.FinalScriptLine.Len())
		{
			StopAnimations();
		}
		
		TextAnimationData.CurrentTextAnimationTime = 0;
	}
}
