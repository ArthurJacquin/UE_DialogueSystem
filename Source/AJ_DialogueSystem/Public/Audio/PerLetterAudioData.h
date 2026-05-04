#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PerLetterAudioData.generated.h"

/**
 * Datatable row struct for per letter audio
 */
USTRUCT(BlueprintType)
struct AJ_DIALOGUESYSTEM_API FAJ_PerLetterAudioData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundWave* LetterAudio;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Pitch = 1.0f;
};
