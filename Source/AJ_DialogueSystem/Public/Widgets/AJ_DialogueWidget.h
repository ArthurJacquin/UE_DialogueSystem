#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "AJ_DialogueSystem/Public/DataAssets/AJ_DialogueSpeakerData.h"
#include "DataAssets/AJ_Dialogue.h"

#include "AJ_DialogueWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;

DECLARE_LOG_CATEGORY_EXTERN(AJ_DialogueWidgetLog, Log, All);

USTRUCT(BlueprintType)
struct FAJ_SpeakerWidgets
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* SpeakerNameText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UImage* SpeakerImage = nullptr;
};

struct FAJ_TextAnimationData
{
public:
	/**
	 * Id of the speaker
	 */
	int32 SpeakerId = -1;
	
	/**
	 * The current dialogue entry
	 */
	FAJ_DialogueEntry DialogueEntry = FAJ_DialogueEntry();
	
	/**
	 * Final script line after animation
	 */
	FString FinalScriptLine = FString();

	/**
	 * Current duration of the text animation
	 */
	float CurrentTextAnimationTime = 0.0f;

	/**
	 * How many characters are displayed
	 */
	int32 CurrentCharacterId = 0;

	/**
	 * Text to be displayed in the script line widget
	 */
	FText CurrentDisplayedText = FText::GetEmpty();
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
	 * Image showing whether a click will go to the next entry or fast forward the animation
	 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* StateImage;

	/**
	 * List of widgets used for each speaker
	 * Set in Blueprint
	 */
	UPROPERTY(BlueprintReadWrite, Category="AJ_DialogueSystem")
	TArray<FAJ_SpeakerWidgets> SpeakerWidgets;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AJ_DialogueSystem|Animations")
	bool bUseAnimations = false;
	
	/**
	 * Whether we should animate the script line or not
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(EditConditionHides, EditCondition="bUseAnimations"), Category = "AJ_DialogueSystem|Animations")
	bool bEnableAnimatedText = false;

	/**
	 * Whether we should use speakers emotions or not
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(EditConditionHides, EditCondition="bUseAnimations"), Category = "AJ_DialogueSystem|Animations")
	bool bEnableTalkingAnimations = false;

	/**
	 * Delay between characters spawn for the text animation, AKA animation speed
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(EditConditionHides, EditCondition="bUseAnimations&&bEnableAnimatedText", Units="s"), Category = "AJ_DialogueSystem|Animations")
	float TimeBetweenCharacters = 0.1f;
	
	/**
	 * Image to show when a click will go to the next entry
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(EditConditionHides, EditCondition="bUseAnimations&&bEnableAnimatedText"), Category = "AJ_DialogueSystem")
	UTexture2D* FastForwardImage;
	
	/**
	 * Color applied on a speaker that is not talking
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AJ_DialogueSystem")
	FLinearColor DisableColor;

	/**
	 * Image to show when a click will go to the next entry
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AJ_DialogueSystem")
	UTexture2D* GoToNextEntryImage;
	
	/**
	 * Is this dialogue completed
	 */
	bool bIsDialogueCompleted;

	/**
	 * Called when the end of the dialogue is reached
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueCompletedDelegate);
	FOnDialogueCompletedDelegate OnDialogueCompleted;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

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
	void ExitDialogue();

	/**
	 * Prepare the widget to display the given entry
	 */
	void SetupDialogueEntry(const FAJ_DialogueEntry& Entry);

	/**
	 * Set who is talking in the dialog
	 * Enable/Disable widgets accordingly
	 */
	void SetSpeaker(const int32& NewSpeakerId, const EAJ_DialogueEmotion& Emotion);

	/**
	 * Get the id of the given speaker into the dialogue (0 or 1)
	 */
	int32 GetSpeakerId(UAJ_DialogueSpeakerData* const InSpeakerData) const;

	/**
	 * Start the animation of the script line + character image animation
	 */
	void StartAnimations();
	
	/**
	 * Start the animation of the script line + character image animation
	 */
	void StopAnimations();

	/**
	 * Update the script line text during animation
	 */
	void UpdateTextAnimation(const float& DeltaTime);
	
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

	/**
	 * Is a text animation currently in progress
	 */
	bool bTextAnimInProgress = false;

	/**
	 * Data for the current text animation
	 */
	FAJ_TextAnimationData TextAnimationData;
};
