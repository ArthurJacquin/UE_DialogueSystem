#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "AJ_DialogueSystemSettings.generated.h"

class UAJ_DialogueWidget;

/**
 * Project settings for the dialogue system
 */
UCLASS(config=Game)
class AJ_DIALOGUESYSTEM_API UAJ_DialogueSystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	//~ Begin UDeveloperSettings interface
	virtual FName GetCategoryName() const;
#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FName GetSectionName() const override;
#endif
	//~ End UDeveloperSettings interface
	
	/*
	 * Class of the dialogue widget to use
	 */
	UPROPERTY(config, EditAnywhere, Category = "Dialogue System")
	TSubclassOf<UAJ_DialogueWidget> DialogueWidgetClass;
};
