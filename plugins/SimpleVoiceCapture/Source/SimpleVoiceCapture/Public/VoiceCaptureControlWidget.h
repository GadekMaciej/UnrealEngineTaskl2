// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VoiceCaptureControlWidget.generated.h"

class UVoiceCaptureComponent;
class UTextBlock;

class UButton;
/**
 * Base widget for controlling voice capture component 
 */
UCLASS()
class SIMPLEVOICECAPTURE_API UVoiceCaptureControlWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	UVoiceCaptureControlWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Voice Capture Widget")
	void ToggleVoiceCapture();
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Voice Capture Widget")
	UButton* ToggleVoiceCaptureButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Voice Capture Widget")
	UTextBlock* ToggleVoiceCaptureTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Voice Capture Widget")
	UTextBlock* CurrentRecordingDurationTextBlock;

protected:
	/** class of voice capture component */
	TSubclassOf<UVoiceCaptureComponent> VoiceCaptureClass;

	/** this widget is not an owner of voice capture component, hence weak pointer */
	TWeakObjectPtr<UVoiceCaptureComponent> VoiceCaptureComponent;

	FText ToggleButtonTextStart;
	FText ToggleButtonTextStop;
};