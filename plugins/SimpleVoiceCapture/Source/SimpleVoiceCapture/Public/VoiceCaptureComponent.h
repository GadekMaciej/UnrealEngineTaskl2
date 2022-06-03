// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VoiceCaptureComponent.generated.h"

/** 
* Actor component that can capture voice from microphone, ideally attached to a player controlled pawn
*/

class UVoiceCaptureComponent;
class IVoiceCapture;
class USoundWaveProcedural;
class UVoiceCaptureControlWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVoiceCaptureFinished, USoundWave*, RecordedAudio);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEVOICECAPTURE_API UVoiceCaptureComponent : public UActorComponent
{
	GENERATED_BODY()
	
friend UVoiceCaptureControlWidget;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	/** delegates */
	UPROPERTY(BlueprintAssignable, Category = "Voice Capture")
	FVoiceCaptureFinished OnVoiceCaptureFinished;

public:	
	// Sets default values for this component's properties
	UVoiceCaptureComponent();

	/** 
	* starts capturing voice 
	* @return whether capturing was successful
	*/
	UFUNCTION(Blueprintcallable, Category = "Voice Capture")
	bool StartCapture();

	/** stops capturing voice */
	UFUNCTION(Blueprintcallable, Category = "Voice Capture")
	void StopCapture();

	float GetCurrentRecordingDuration() const {return CurrentRecordingDuration;}
	bool GetIsCurrentlyRecording() const {return bIsCurrentlyRecording;}
	TSharedPtr<IVoiceCapture> GetVoiceCapture() const {return VoiceCapture;}
	
	/** max buffer size for captured data */
	int32 MaxRawCaptureDataSize;
	
	/** array of captured data */
	TArray<uint8> RawCaptureData;

	/** whether was playing in last tick */
	bool bLastWasPlaying = false;
	
	int32 StarvedDataCount;

	/** Deprecated */
	TArray<uint8> ZeroFilledArray;
	
protected:
	/** create and configure configure SoundWaveProcedural */
	void InitSoundWaveProcedural();
	
	UPROPERTY(BlueprintReadOnly, Category="Voice Capture Widget")
	float CurrentRecordingDuration;

	UPROPERTY(BlueprintReadOnly, Category="Voice Capture Widget")
	bool bIsCurrentlyRecording;
	
	TSharedPtr<IVoiceCapture> VoiceCapture;

	// // prevented from being garbage collected by `AddToRoot()`
	// UPROPERTY(BlueprintReadOnly)
	// UAudioComponent* VoiceComponent;

	/** Internal reference to audio component's streaming wave object */
	UPROPERTY()
	USoundWaveProcedural* ProceduralSoundWav;
private:
};
