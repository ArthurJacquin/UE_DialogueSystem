#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "AJ_DialogueSpeakerData.generated.h"

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
	TSoftObjectPtr<UTexture2D> BaseImage;
};
