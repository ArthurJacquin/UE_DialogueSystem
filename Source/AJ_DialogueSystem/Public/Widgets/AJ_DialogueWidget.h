#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "AJ_DialogueWidget.generated.h"

class UAJ_Dialogue;
class UAJ_DialogueSpeakerData;
class UButton;
class UImage;
class UTextBlock;
struct FAJ_DialogueEntry;

DECLARE_LOG_CATEGORY_EXTERN(AJ_DialogueWidgetLog, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueCompletedDelegate);

USTRUCT(BlueprintType)
struct FAJ_SpeakerWidgets
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* SpeakerNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UImage* SpeakerImage;
};

/**
 * Main widget for the dialogue system, takes the whole screen
 * Supports 1 or 2 speakers
 */
UCLASS(ClassGroup = "AJ_DialogueSystem")
class AJ_DIALOGUESYSTEM_API UAJ_DialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Play the given dialogue in this widget. Do not call directly, use UAJ_DialogueSystemUtilities::PlayDialogue instead
	 **/
	void PlayDialogue(UAJ_Dialogue* const InDialogue);

	/**
	 * Assign the widgets to their respective speakers in SpeakerWidgets, the order matters.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void AssignSpeakersWidgets();
	
	/**
	 * Called when a new speaker starts to speak
	 * Handle transition between speaking and non-speaking states
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnSpeakerStateChanged(bool bIsSpeaking, const FAJ_SpeakerWidgets& InSpeakerWidgets, const UAJ_DialogueSpeakerData* SpeakerData);
	virtual void OnSpeakerStateChanged_Implementation(bool bIsSpeaking, const FAJ_SpeakerWidgets& InSpeakerWidgets, const UAJ_DialogueSpeakerData* SpeakerData);

	/**
	 * The script line being spoken
	 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ScriptLineText;

	/**
	 * The button to go to the next dialogue entry
	 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ContinueButton;

	/**
	 * List of widgets used for each speaker
	 * Set in Blueprint
	 */
	UPROPERTY(BlueprintReadWrite, Category="AJ_DialogueSystem")
	TArray<FAJ_SpeakerWidgets> SpeakerWidgets;

	/**
	 * Color applied on a speaker that is not talking
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AJ_DialogueSystem")
	FLinearColor DisableColor;

	/**
	 * Called when the end of the dialogue have been reached
	 */
	UPROPERTY(BlueprintAssignable)
	FOnDialogueCompletedDelegate OnDialogueCompleted;

protected:
	virtual void NativeConstruct() override;

private:
	/**
	 * Cache the data necessary for this widget to work
	 */
	void CacheDialogueData();

	/**
	 * Setup the speakers widgets from the dialogue's SpeakerData
	 */
	void SetupSpeakersWidgets();

	/**
	 * Go to the next dialogue entry
	 */
	UFUNCTION()
	void ContinueDialogue();

	/**
	 * Close the dialogue
	 */
	void ExitDialogue(bool bCallCompletionEvent);

	/**
	 * Prepare the widget to display the given entry
	 */
	void SetupDialogueEntry(const FAJ_DialogueEntry& Entry);

	/**
	 * Set who is talking in the dialog
	 * Enable/Disable widgets accordingly
	 */
	void SetSpeaker(const int32& NewSpeakerId);

	/**
	 * Get the id of the given speaker into the dialogue (0 or 1)
	 */
	int32 GetSpeakerId(UAJ_DialogueSpeakerData* const InSpeakerData) const;

	/**
	 * The dialogue being played
	 */
	UAJ_Dialogue* Dialogue;

	/**
	 * The index of the dialogue entry currently shown
	 */
	int32 CurrentEntryId;

	/**
	 * List of speakers for this dialogue
	 */
	TArray<UAJ_DialogueSpeakerData*> Speakers;
};
