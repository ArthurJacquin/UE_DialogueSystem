#pragma once

#include "AJ_DialogueSpeakerData.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "AJ_Dialogue.generated.h"

/*
* A single entry within the dialogue, contains data about the speaker and the line spoken
*/
USTRUCT(BlueprintType)
struct FAJ_DialogueEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UAJ_DialogueSpeakerData* SpeakerData = nullptr;
	
	UPROPERTY(EditAnywhere)
	EAJ_DialogueEmotion Emotion = EAJ_DialogueEmotion::Neutral;

	UPROPERTY(EditAnywhere)
	FText ScriptLine = FText();
};

/**
 * Data asset storing all the data for a dialogue
 */
UCLASS(ClassGroup = "AJ_DialogueSystem")
class AJ_DIALOGUESYSTEM_API UAJ_Dialogue : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * The entries for this dialogue
	 */
	UPROPERTY(EditAnywhere)
	TArray<FAJ_DialogueEntry> DialogueEntries;
};
