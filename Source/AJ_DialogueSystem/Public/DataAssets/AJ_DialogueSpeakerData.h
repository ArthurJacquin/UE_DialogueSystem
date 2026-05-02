#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "AJ_DialogueSpeakerData.generated.h"

UENUM(BlueprintType)
enum class EAJ_DialogueEmotion : uint8
{
	Neutral,
	Happy,
	Sad,
	Scared,
	Thinking,
};

USTRUCT(BlueprintType)
struct FAJ_SpeakerEmotionTextures
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> DefaultImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> TalkingFlipbook;
};

/**
 * Data asset storing the data for a character that can be part of a dialogue
 * Inherit from this class + your base character class to create a speaker data asset class
 */
UCLASS(ClassGroup = "AJ_DialogueSystem")
class AJ_DIALOGUESYSTEM_API UAJ_DialogueSpeakerData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Speaker")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Speaker")
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Speaker")
	TMap<EAJ_DialogueEmotion, FAJ_SpeakerEmotionTextures> ImagesPerEmotion;
};
