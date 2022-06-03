// Fill out your copyright notice in the Description page of Project Settings.


#include "VoiceCaptureComponent.h"

#include "AudioDevice.h"
#include "OnlineSubsystemUtils.h"
#include "SimpleVoiceCapture.h"
#include "VoiceModule.h"

// Sets default values for this component's properties
UVoiceCaptureComponent::UVoiceCaptureComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	CurrentRecordingDuration = 0.f;
	bIsCurrentlyRecording = false;
}

bool UVoiceCaptureComponent::StartCapture()
{
	bool bIsStartCaptureSuccess = false;
	
	if (VoiceCapture == nullptr)
	{
			VoiceCapture = FVoiceModule::Get().CreateVoiceCapture("");
	}
	
	ensureMsgf(VoiceCapture.IsValid(), TEXT("Voice Capture Could not be created properly"));
	
	if (VoiceCapture.IsValid())
	{
		if (ProceduralSoundWav)
		{
			ProceduralSoundWav->ResetAudio();
		}
		
		MaxRawCaptureDataSize = VoiceCapture->GetBufferSize();

		RawCaptureData.Empty(MaxRawCaptureDataSize);
		RawCaptureData.AddUninitialized(MaxRawCaptureDataSize);

		bIsCurrentlyRecording = true;
		bIsStartCaptureSuccess = true;
		
		VoiceCapture->Start();
		UE_LOG(LogSimpleVoiceCapture, Verbose, TEXT("Voice capture started"));
	}
	return bIsStartCaptureSuccess;
}


void UVoiceCaptureComponent::StopCapture()
{
	if (VoiceCapture)
	{
		
		bIsCurrentlyRecording = false;
		OnVoiceCaptureFinished.Broadcast(ProceduralSoundWav);
		VoiceCapture->Stop();
		CurrentRecordingDuration = 0.f;
		UE_LOG(LogSimpleVoiceCapture, Verbose, TEXT("Voice capture stopped"));
	}
}

// Called when the game starts
void UVoiceCaptureComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}

// Called every frame
void UVoiceCaptureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsCurrentlyRecording && VoiceCapture)
	{
		CurrentRecordingDuration += DeltaTime;
		if (ProceduralSoundWav == nullptr)
		{
			InitSoundWaveProcedural();
			
			// Bind the GenerateData callback with FOnSoundWaveProceduralUnderflow object
			ProceduralSoundWav->OnSoundWaveProceduralUnderflow
			.BindLambda([this](USoundWaveProcedural* InProceduralWave, int32 SamplesRequired)
			{
				if (!bIsCurrentlyRecording)
				{
					UE_LOG(LogSimpleVoiceCapture, Warning, TEXT("Procedural sound wave underflow - no audio queued"))
				}
			});
		}
		if (ProceduralSoundWav)
		{
			check(ProceduralSoundWav);
		
			uint32 VoiceCaptureReadBytes = 0;
			uint32 VoiceCaptureAvailableBytes = 0;

			// get number of available bytes
			EVoiceCaptureState::Type CaptureState = VoiceCapture->GetCaptureState(VoiceCaptureAvailableBytes);
			
			// fill the buffer with microphone voice data
			VoiceCapture->GetVoiceData(RawCaptureData.GetData(),VoiceCaptureAvailableBytes, VoiceCaptureReadBytes);

			// deprecated:: I tried to fill empty spaces with silence - (0), but it made the actual recorded voice very choppy
			
			/*
			if (CaptureState == EVoiceCaptureState::Type::NoData)
			{
				// file length = sample rate * number of channels * (bits per sample / 8) * time
				uint32 FileLength = (16000 * 1	* (16 / 8)) * DeltaTime;
				FileLength -= FileLength % 2;
				
				ProceduralSoundWav->QueueAudio(ZeroFilledArray.GetData(), FileLength);
			}
			else
			{
				ProceduralSoundWav->QueueAudio(RawCaptureData.GetData(), VoiceCaptureReadBytes);
			}
			*/

			ProceduralSoundWav->Duration = CurrentRecordingDuration;
			ProceduralSoundWav->QueueAudio(RawCaptureData.GetData(), VoiceCaptureReadBytes);

			//deprecated:: was useful when this class still had an audio component,
			//then I realised the task didn't mention that it should actually have one
			
			// bool bIsPlaying = VoiceComponent->IsPlaying();
			// if (bIsPlaying != bLastWasPlaying)
			// {
			// 	UE_LOG(LogVoice, Log, TEXT("VOIP audio component %s playing!"), bIsPlaying ? TEXT("is") : TEXT("is not"));
			// 	bLastWasPlaying = bIsPlaying;
			// }
			//
			// StarvedDataCount = (!bIsPlaying || (ProceduralSoundWav->GetAvailableAudioByteCount() != 0)) ? 0 : (StarvedDataCount + 1);
			// if (StarvedDataCount > 1)
			// {
			// 	UE_LOG(LogVoice, Log, TEXT("VOIP audio component starved %d frames!"), StarvedDataCount);
			// }
		}
	}
}

void UVoiceCaptureComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	
	if (VoiceCapture)
	{
		VoiceCapture->Shutdown();
	}
	
	VoiceCapture = nullptr;
}

void UVoiceCaptureComponent::InitSoundWaveProcedural()
{
	ProceduralSoundWav = NewObject<USoundWaveProcedural>();
	
	ProceduralSoundWav->SetSampleRate(16000);
	ProceduralSoundWav->NumChannels = 1;
	
	// later this duration is changed to actual duration of the recorded audio
	ProceduralSoundWav->Duration = INDEFINITELY_LOOPING_DURATION;
	
	ProceduralSoundWav->SoundGroup = SOUNDGROUP_Voice;
	ProceduralSoundWav->bLooping = false;
	ProceduralSoundWav->bProcedural = true;
	ProceduralSoundWav->Pitch = 1.f;
	ProceduralSoundWav->Volume = 1.f;

	// deprecated
	ZeroFilledArray.Init(0, 100000);
}


