// Fill out your copyright notice in the Description page of Project Settings.


#include "VoiceCaptureControlWidget.h"

#include "SimpleVoiceCapture.h"
#include "VoiceCaptureComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

UVoiceCaptureControlWidget::UVoiceCaptureControlWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	VoiceCaptureClass = UVoiceCaptureComponent::StaticClass();
	ToggleButtonTextStart = FText::FromString(FString(TEXT("Start Capturing")));
	ToggleButtonTextStop = FText::FromString(FString(TEXT("Stop Capturing")));
}

void UVoiceCaptureControlWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (VoiceCaptureComponent.IsValid())
	{
		float RecordingDuration = VoiceCaptureComponent.Get()->CurrentRecordingDuration;
		FText RecordingDurationText = FText::FromString(FString::Printf(TEXT("Current Recording Duration: %f"), RecordingDuration));
		CurrentRecordingDurationTextBlock->SetText(RecordingDurationText);
	}
}

void UVoiceCaptureControlWidget::ToggleVoiceCapture()
{
	// should always pass, since we don't bind this function in NativeOnInitialized when there is no component
	check(VoiceCaptureComponent.IsValid());

	if (VoiceCaptureComponent.Get()->bIsCurrentlyRecording)
	{
		VoiceCaptureComponent.Get()->StopCapture();
		ToggleVoiceCaptureTextBlock->SetText(ToggleButtonTextStart);
	}
	else
	{
		VoiceCaptureComponent.Get()->StartCapture();
		ToggleVoiceCaptureTextBlock->SetText(ToggleButtonTextStop);
	}
}

void UVoiceCaptureControlWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	VoiceCaptureComponent = CastChecked<UVoiceCaptureComponent>(GetOwningPlayerPawn()->FindComponentByClass(VoiceCaptureClass));

	if (!VoiceCaptureComponent.IsValid())
	{
		UE_LOG(LogSimpleVoiceCapture, Warning, TEXT("Could not find Voice Capture component on owner player pawn"))
		return;
	}

	ToggleVoiceCaptureButton->OnClicked.AddDynamic(this, &UVoiceCaptureControlWidget::ToggleVoiceCapture);
	UE_LOG(LogSimpleVoiceCapture, Verbose, TEXT("Successfuly found owner player Voice Capture Component"))
}