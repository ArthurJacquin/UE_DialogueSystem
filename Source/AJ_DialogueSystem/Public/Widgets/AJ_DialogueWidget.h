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

/**
 * Main widget for the dialogue system, takes the whole screen
 */
UCLASS(ClassGroup = "AJ_DialogueSystem")
class AJ_DIALOGUESYSTEM_API UAJ_DialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Play the given dialogue in this widget. Do not call directly, use UAJ_DialogueSystemUtilities::PlayDialogue instead
	 **/
	void PlayDialogue(UAJ_Dialogue* InDialogue);

	/**
	 * The name of the first speaker
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Speaker1NameText;

	/**
	 * The name of the second speaker
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Speaker2NameText;

	/**
	 * The image of the first speaker
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Speaker1Image;

	/**
	 * The image of the second speaker
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Speaker2Image;

	/**
	 * The script line being spoken
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ScriptLineText;

	/**
	 * The button to go to the next dialogue entry
	 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ContinueButton;

protected:
	virtual void NativeConstruct() override;

	/**
	 * The dialogue being played
	 */
	UPROPERTY(BlueprintReadOnly)
	UAJ_Dialogue* Dialogue;

private:
	/**
	 * Go to the next dialogue entry
	 */
	UFUNCTION()
	void ContinueDialogue();

	/**
	 * Close the dialogue
	 */
	void ExitDialogue(bool bCallEvent);

	/**
	 * Prepare the widget to display the given entry
	 */
	void SetupDialogueEntry(const FAJ_DialogueEntry& Entry);

	/**
	 * The index of the dialogue entry currently shown
	 */
	int32 CurrentEntryId;
};
